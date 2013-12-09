#include "fwInputHandler.h"

#include "fwPacketQueue.h"
#include "fwFrameQueue.h"
#include "fwPacketQueue.h"

static uint8_t		codec_type(enum CodecID cid);

InputHandler::InputHandler(const char *sdp, int width, int height, AVPixelFormat pix_fmt) {
    fmt_ctx = NULL;
    video_dec_ctx = NULL;
    video_dec = NULL;
    video_stream = NULL;
    frame = NULL;
    frameRGB = NULL;
    sws_ctx = NULL;
    bsf = NULL;
    m_PictureBuffer = NULL;
    io_context = NULL;

    m_Width = width;
    m_Height = height;
    m_PixFmt = pix_fmt;

    m_FrameTimer = (double)av_gettime() / 1000000.0;
    m_LastPTS = 0.0;
    m_LastDelay = 40e-3;
    m_VideoClock = 0.0;
    m_FPS = 20.0f;
    sdp_desc = (char*)av_malloc(strlen(sdp)+1);
    sdp_desc[strlen(sdp)] = 0;
    io_buffer = 0;
    strcpy(sdp_desc, sdp);
}

double InputHandler::SyncVideo(AVFrame* fr, double pts) {
    double frame_delay;

    if(pts != 0) {
        // if we have pts, set video clock to it
        m_VideoClock = pts;
    } else {
        // if we aren't given a pts, set it to the clock
        pts = m_VideoClock;
    }

    // update the video clock */
    frame_delay = av_q2d(video_stream->codec->time_base);
    // if we are repeating a frame, adjust clock accordingly
    frame_delay += fr->repeat_pict * (frame_delay * ((double)1/video_stream->codec->ticks_per_frame));
    
    m_VideoClock += frame_delay;

    return pts;
}

typedef struct FLOWStream {
    PacketQueue* buffer;
    char sdp[1024];
} FLOWStream;

static bool first = true;
static int ReadFunc(void *opaque, uint8_t *buf, int buf_size) {
    //printf("WIN!!!!!! %d\n", buf_size);
    FLOWStream* nfo = (FLOWStream*) opaque;
    char* sdp = nfo->sdp;
    if(first) {
        //printf("SDP!!!!!! %s %s\n", sdp, nfo->sdp);
        strcpy((char*)buf, sdp);
        //printf("SDP!!!!!! %s \n", (char*)buf);
        first = false;

        return strlen(sdp);
    }
    FLOWPacket pkt;
    int written_size = 0;
    PacketQueue* q = nfo->buffer;
    //int ret = q->get(&pkt);
    //int i = 0;
    //if(ret!=1) {
    //    i++;
        //memcpy(buf+written_size, pkt.m_Data, pkt.m_Size);
        //written_size += pkt.m_Size;
    //}
    //printf("Written !!!!!! %d\n", written_size);
    return written_size;
}

static bool inputExhausted = false;

static int ReadRTPFunc(void *opaque, uint8_t *buf, int buf_size) {
    //printf("WIN 2!!!!!! %d\n", buf_size);
    FLOWPacket pkt;
    int written_size = 0;
    PacketQueue* q = (PacketQueue*)opaque;
    int ret = q->get(&pkt);
    int i = 0;
    //printf("Queue Size %d %d\n", q->m_PlayoutPackets->size(), pkt.m_SeqNum);
    if ( (ret != 1) && (written_size + pkt.m_Size < 1473)) {
        i++;
        memcpy(buf+written_size, pkt.m_Data, pkt.m_Size);
        written_size += pkt.m_Size;
        //delete pkt.m_Data;
        //ret = q->get(&pkt);
    }
    
    //printf("Written !!!!!! %d %d %d\n", written_size, buf_size, ret);
    if ( written_size == 0 ) {
        inputExhausted = true;
    }
    return written_size;
}

static int WriteRTPFunc(void *opaque, uint8_t *buf, int buf_size) {
    //printf("WIN 3!!!!!! %d\n", buf_size);
    
    return 0;
}

#define IO_BUFFER_SIZE 262144

static uint64_t global_video_pkt_pts = 0;

static int flow_get_buffer(struct AVCodecContext *c, AVFrame *pic) {
  int ret = avcodec_default_get_buffer(c, pic);
  uint64_t *pts = (uint64_t*)av_malloc(sizeof(uint64_t));
  *pts = global_video_pkt_pts;
  pic->opaque = pts;

  //printf("Called malloc buffer\n");

  return ret;
}

static void flow_release_buffer(struct AVCodecContext *c, AVFrame *pic) {
  if(pic) av_freep(&pic->opaque);
  avcodec_default_release_buffer(c, pic);
}

bool InputHandler::NextChunk(FrameQueue* fq) {
    AVPacket pkt;
    double pts = 0;
    av_init_packet(&pkt);
    //printf("Came along\n");
    int frameFinished = 0;

    while ( (av_read_frame(fmt_ctx, &pkt) >= 0) ) {
        global_video_pkt_pts = pkt.pts;
        if (avcodec_decode_video2(video_dec_ctx, frame, &frameFinished, &pkt) < 0 ) {
            av_free_packet(&pkt);
            fprintf(stderr, "Error decoding packet ??????????????????????????????\n");
            break;
        }

        if(pkt.dts == AV_NOPTS_VALUE 
	  && frame->opaque && *(uint64_t*)frame->opaque != AV_NOPTS_VALUE) {
	  pts = *(uint64_t *)frame->opaque;
        } else if(pkt.dts != AV_NOPTS_VALUE) {
	  pts = pkt.dts;
        } else {
	  pts = 0;
        }

        pts *= av_q2d(video_stream->time_base);
        //printf("PTS : %f\n", pts);

        if(frameFinished) {
	  //printf("PTS : %f\n", pts);
	  pts = SyncVideo(frame, pts);
	  //printf("PTS : %f\n", pts);
	  fq->put(frame, pts, video_dec_ctx->pix_fmt, video_dec_ctx->width, video_dec_ctx->height);
        }
        av_free_packet(&pkt);
        //printf("Came in\n");
        
    }
    inputExhausted = true;
    //printf("Came ?????????????????????????????????????\n");
    //printf("!!!!!! Next Input %d\n", fq->m_NextInputFrameIndex);
    fq->swapQueues();
    return true;
}

bool InputHandler::Open() {
    int i, ret;
    int video_streams = 0, audio_streams = 1;
          
    av_register_all();
    avformat_network_init();
    //av_log_set_level(AV_LOG_DEBUG);
    
    fmt_ctx = avformat_alloc_context();
    if(!fmt_ctx) {
        return false;
    }
    io_buffer = (BYTE*)av_malloc((IO_BUFFER_SIZE+FF_INPUT_BUFFER_PADDING_SIZE)*sizeof(BYTE));
    //BYTE* sdp_buffer = (BYTE*)av_malloc((strlen(sdp_desc)+FF_INPUT_BUFFER_PADDING_SIZE)*sizeof(BYTE));
    //FLOWStream *nfo = new FLOWStream;
    //nfo->buffer = in_buffer;
    //strcpy(nfo->sdp, sdp_desc);
    frame = avcodec_alloc_frame();
    sdp_io_context = avio_alloc_context((unsigned char*)sdp_desc, strlen(sdp_desc), 0, NULL, NULL, NULL, NULL);
    //sdp_io_context = avio_alloc_context(sdp_buffer, strlen(sdp_desc), 0, nfo, &ReadFunc, NULL, NULL); 
    //avio_open2(&sdp_io_context, "", AVIO_FLAG_READ, NULL, NULL);
    //strcpy((char*)sdp_buffer, sdp_desc);
    if(!sdp_io_context)
        return false;
    //sdp_io_context->seekable = 0;
    //sdp_io_context->write_flag = 0;

    //AVProbeData probeData;
    //probeData.buf = sdp_buffer;
    //probeData.buf_size = strlen(sdp_desc);
    //probeData.filename = "";
    fmt_ctx->pb = sdp_io_context;
    //fmt_ctx->flags = AVFMT_FLAG_CUSTOM_IO;
    //fmt_ctx->probesize = strlen(sdp_desc);
    fmt_ctx->iformat = av_find_input_format("sdp");

    AVDictionary *options = NULL;
    av_dict_set(&options, "sdp_flags", "custom_io", 0);
    //av_dict_set(&options, "rtsp_flags", "custom_io", 0);
    int err = avformat_open_input(&fmt_ctx, "", fmt_ctx->iformat, &options);
    //fmt_ctx->iformat->
    io_context = avio_alloc_context(io_buffer, IO_BUFFER_SIZE, 1, in_buffer, &ReadRTPFunc, &WriteRTPFunc, NULL); 
    //avio_open2(&io_context, "", AVIO_FLAG_READ_WRITE, NULL, NULL);
    if(!io_context)
        return false;
    //printf("Where are you\n");
    io_context->seekable = 0;
    //AVInputFormat* fr = av_find_input_format("rtp");

    //io_context->write_flag = 0;
    //avio_close(fmt_ctx->pb);
    //fr->flags = 0;
    fmt_ctx->pb = io_context;
    //fmt_ctx->flags = AVFMT_FLAG_CUSTOM_IO;
    //fmt_ctx->iformat = fr;
    //strcpy(fmt_ctx->filename,"127.0.0.1//54444");
    //fmt_ctx->priv_data->transport = RTSP_LOWER_TRANSPORT_CUSTOM;
    //fmt_ctx->pb->eof_reached = FALSE;
    video_dec_ctx = fmt_ctx->streams[0]->codec;
    video_dec_ctx->width = m_Width;
    video_dec_ctx->height = m_Height;
    video_dec_ctx->pix_fmt = m_PixFmt;
    video_stream = fmt_ctx->streams[0];

    frameRGB = avcodec_alloc_frame();
    
    // Determine required buffer size and allocate buffer
    m_NumBytes=avpicture_get_size(PIX_FMT_RGB24, 2000,
    2000);
    m_PictureBuffer=new BYTE[m_NumBytes];
    avpicture_fill((AVPicture *)frameRGB, m_PictureBuffer, PIX_FMT_RGB24, 2000, 2000);
    
    sws_ctx = sws_getContext (
     video_dec_ctx->width, video_dec_ctx->height,
     video_dec_ctx->pix_fmt,
     video_dec_ctx->width, video_dec_ctx->height,
     PIX_FMT_RGB24, SWS_FAST_BILINEAR,
     NULL, NULL, NULL );

    video_dec = avcodec_find_decoder(video_dec_ctx->codec_id);
    if (!video_dec) {
        printf("Failed to find %s codec\n",
                "H264");
        return false;
    }
    if ((ret = avcodec_open2(video_dec_ctx, video_dec, NULL)) < 0) {
        printf("Failed to open %s codec\n",
                    "H264");
        return false;
    }

    video_dec_ctx->get_buffer = flow_get_buffer;
    video_dec_ctx->release_buffer = flow_release_buffer;

    printf("Here\n");
    av_dump_format(fmt_ctx, 0, "", 0);
    return true;
}

void InputHandler::Close() {
    if(video_dec_ctx) avcodec_close(video_dec_ctx);
    //if(pCodecCtx) avcodec_close(pCodecCtx);
    //if(bsf) av_bitstream_filter_close(bsf);
    //if(out_ctx) avformat_free_context(out_ctx);
    if(fmt_ctx) avformat_close_input(&fmt_ctx);
    if(sws_ctx) sws_freeContext(sws_ctx);
    if(bsf) av_bitstream_filter_close(bsf);
    if(sdp_desc) av_free(sdp_desc);
    if(io_buffer) av_free(io_buffer);
    if(m_PictureBuffer) delete m_PictureBuffer;
    avcodec_free_frame(&frame);
    avcodec_free_frame(&frameRGB);
}

static uint8_t codec_type(enum CodecID cid)
{
  switch (cid) {
    case CODEC_ID_MPEG1VIDEO:
    case CODEC_ID_MPEG2VIDEO:
      return 1;
    case CODEC_ID_H261:
      return 2;
    case CODEC_ID_H263P:
    case CODEC_ID_H263:
      return 3;
    case CODEC_ID_MJPEG:
      return 4;
    case CODEC_ID_MPEG4:
      return 5;
    case CODEC_ID_FLV1:
      return 6;
    case CODEC_ID_SVQ3:
      return 7;
    case CODEC_ID_DVVIDEO:
      return 8;
    case CODEC_ID_H264:
      return 9;
    case CODEC_ID_THEORA:
    case CODEC_ID_VP3:
      return 10;
    case CODEC_ID_SNOW:
      return 11;
    case CODEC_ID_VP6:
      return 12;
    case CODEC_ID_DIRAC:
      return 13;
    case CODEC_ID_MP2:
    case CODEC_ID_MP3:
      return 129;
    case CODEC_ID_AAC:
      return 130;
    case CODEC_ID_AC3:
      return 131;
    case CODEC_ID_VORBIS:
      return 132;
    default:
      fprintf(stderr, "Unknown codec ID %d\n", cid);
      return 0;
  }
}