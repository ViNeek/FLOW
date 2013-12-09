#include "fwInputHandler.h"

#include "fwPacketQueue.h"
#include "fwFrameQueue.h"
#include "fwOutputHandler.h"
#include "fwPacketQueue.h"

static uint8_t		codec_type(enum CodecID cid);

InputHandler::InputHandler() {
    fmt_ctx = NULL;
    video_dec_ctx = NULL;
    video_dec = NULL;
    video_stream = NULL;
    frame = NULL;
    frameRGB = NULL;
    sws_ctx = NULL;
    bsf = NULL;
    m_PictureBuffer = NULL;

    m_FrameTimer = (double)av_gettime() / 1000000.0;
    m_LastPTS = 0.0;
    m_LastDelay = 40e-3;
    m_VideoClock = 0.0;
    m_FPS = 20.0f;
}

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

static double preValid = 0.0f;
double InputHandler::SyncVideo(AVFrame* fr, double pts) {
    double frame_delay;
    frame_delay = 2*av_q2d(video_stream->codec->time_base);

    if(pts != 0) {
        // if we have pts, set video clock to it
        if(pts > (m_VideoClock+frame_delay) ) {
	  pts = preValid+frame_delay;
	  preValid =pts;
        }
        m_VideoClock = pts;
    } else {
        // if we aren't given a pts, set it to the clock
        pts = m_VideoClock;
    }

    //printf("Max FPS %g\n", pts);
    // update the video clock */
    
    // if we are repeating a frame, adjust clock accordingly
    frame_delay += fr->repeat_pict * (frame_delay * ((double)1/video_stream->codec->ticks_per_frame));
    
    m_VideoClock += frame_delay;

    if(pts > (m_LastPTS+frame_delay) ) {
        //pts = m_LastPTS+frame_delay;
    }

    return pts;
}

bool InputHandler::InitStream(FrameQueue* fq, PacketQueue* buffer, OutputHandler* out) {
    int maxFpsCount = m_FPS+0.5;
    //maxFpsCount++;
    m_MaxFrameCount = maxFpsCount;

    int frameFinished = false;
    int packetCount = 0;
    AVPacket pkt;
    double pts = 0;
    static int bitrate = 0;
    av_init_packet(&pkt);
    //printf("Max FPS %d\n", video_stream->codec->ticks_per_frame);
    for(int frameCount = 0; frameCount < m_MaxFrameCount; ) {
        if (av_read_frame(fmt_ctx, &pkt) >= 0) {
            if (pkt.stream_index == 0) {
                pts = 0;

                global_video_pkt_pts = pkt.pts;   

                if (avcodec_decode_video2(video_dec_ctx, frame, &frameFinished, &pkt) < 0 ) {
                    fprintf(stderr, "Error decoding packet\n");
                }

                //in_pq->put(&pkt);
                if(pkt.dts == AV_NOPTS_VALUE 
                    && frame->opaque && *(uint64_t*)frame->opaque != AV_NOPTS_VALUE) {
                    pts = *(uint64_t *)frame->opaque;
                } else if(pkt.dts != AV_NOPTS_VALUE) {
                    pts = pkt.dts;
                } else {
                    pts = 0;
                }

                pts *= av_q2d(video_stream->time_base);

                stream_convert(&pkt);
                if (pkt.pts != AV_NOPTS_VALUE) {
                    pkt.pts = av_rescale_q(pkt.pts, video_stream->time_base, out->video_stream->time_base);
                }
                if (pkt.dts != AV_NOPTS_VALUE) {
                    pkt.dts = av_rescale_q(pkt.dts, video_stream->time_base, out->video_stream->time_base);
                }
                if (pkt.duration != 0) {
                    pkt.duration = av_rescale_q(pkt.duration, video_stream->time_base, out->video_stream->time_base);
                }
                if (av_write_frame(out->fmt_ctx, &pkt) < 0)
                    printf("MITSOS vre eisai!!!!\n");
                BYTE *ptr;
                int chunkSize = avio_close_dyn_buf(out->fmt_ctx->pb,&ptr);
	      bitrate += chunkSize;
                //printf("Size %d\n",chunkSize);
                // Save global pts to be stored in pFrame in first call
                buffer->ProcessChunk(ptr, chunkSize); 
                av_free(ptr);
                avio_open_dyn_buf(&out->fmt_ctx->pb);
                out->fmt_ctx->pb->max_packet_size = 1472;

                packetCount++;
                if(frameFinished) {
                    frameCount++;
                    pts = SyncVideo(frame, pts);
                    fq->put(frame, pts, video_dec_ctx->pix_fmt, video_dec_ctx->width, video_dec_ctx->height);
                    packetCount = 0;
                }
            }
        }
        av_free_packet(&pkt);
    }
    buffer->UpdateEndTimestamp();
    fq->swapQueues();
    buffer->swapQueues();
    for(int frameCount = 0; frameCount < m_MaxFrameCount; ) {
        if (av_read_frame(fmt_ctx, &pkt) >= 0) {
            if (pkt.stream_index == video_stream_idx) {
                pts = 0;
                
                // Save global pts to be stored in pFrame in first call
                global_video_pkt_pts = pkt.pts;
                
                if (avcodec_decode_video2(video_dec_ctx, frame, &frameFinished, &pkt) < 0 ) {
                    fprintf(stderr, "Error decoding packet\n");
                }

                if(pkt.dts == AV_NOPTS_VALUE 
                    && frame->opaque && *(uint64_t*)frame->opaque != AV_NOPTS_VALUE) {
                    pts = *(uint64_t *)frame->opaque;
                } else if(pkt.dts != AV_NOPTS_VALUE) {
                    pts = pkt.dts;
                } else {
                    pts = 0;
                }
                pts *= av_q2d(video_dec_ctx->time_base);

                stream_convert(&pkt);
                if (pkt.pts != AV_NOPTS_VALUE) {
                    pkt.pts = av_rescale_q(pkt.pts, video_stream->time_base, out->video_stream->time_base);
                }
                if (pkt.dts != AV_NOPTS_VALUE) {
                    pkt.dts = av_rescale_q(pkt.dts, video_stream->time_base, out->video_stream->time_base);
                }
                if (pkt.duration != 0) {
                    pkt.duration = av_rescale_q(pkt.duration, video_stream->time_base, out->video_stream->time_base);
                }
                if (av_write_frame(out->fmt_ctx, &pkt) < 0)
                    printf("MITSOS vre eisai!!!!\n");
                BYTE *ptr;
                int chunkSize = avio_close_dyn_buf(out->fmt_ctx->pb,&ptr);
                //printf("Size %d\n",chunkSize);
                // Save global pts to be stored in pFrame in first call
                buffer->ProcessChunk(ptr, chunkSize); 
                av_free(ptr);
                avio_open_dyn_buf(&out->fmt_ctx->pb);
                out->fmt_ctx->pb->max_packet_size = 1472;

                packetCount++;
                if(frameFinished) {
                    frameCount++;
                    pts = SyncVideo(frame, pts);
                    fq->put(frame, pts, video_dec_ctx->pix_fmt, video_dec_ctx->width, video_dec_ctx->height);
                    packetCount = 0;
                }
            }
        }
        av_free_packet(&pkt);
    }
    buffer->UpdateEndTimestamp();
    fq->swapQueues();
    buffer->swapQueues();
    for(int frameCount = 0; frameCount < m_MaxFrameCount; ) {
        if (av_read_frame(fmt_ctx, &pkt) >= 0) {
            if (pkt.stream_index == video_stream_idx) {
                pts = 0;

                // Save global pts to be stored in pFrame in first call
                global_video_pkt_pts = pkt.pts;

                if (avcodec_decode_video2(video_dec_ctx, frame, &frameFinished, &pkt) < 0 ) {
                    fprintf(stderr, "Error decoding packet\n");
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

                stream_convert(&pkt);
                if (pkt.pts != AV_NOPTS_VALUE) {
                    pkt.pts = av_rescale_q(pkt.pts, video_stream->time_base, out->video_stream->time_base);
                }
                if (pkt.dts != AV_NOPTS_VALUE) {
                    pkt.dts = av_rescale_q(pkt.dts, video_stream->time_base, out->video_stream->time_base);
                }
                if (pkt.duration != 0) {
                    pkt.duration = av_rescale_q(pkt.duration, video_stream->time_base, out->video_stream->time_base);
                }
                if (av_write_frame(out->fmt_ctx, &pkt) < 0)
                    printf("MITSOS vre eisai!!!!\n");
                BYTE *ptr;
                int chunkSize = avio_close_dyn_buf(out->fmt_ctx->pb,&ptr);
                //printf("Size %d\n",chunkSize);
                // Save global pts to be stored in pFrame in first call
                buffer->ProcessChunk(ptr, chunkSize); 
                av_free(ptr);
                avio_open_dyn_buf(&out->fmt_ctx->pb);
                out->fmt_ctx->pb->max_packet_size = 1472;

                packetCount++;
                if(frameFinished) {
                    frameCount++;
                    pts = SyncVideo(frame, pts);
                    fq->put(frame, pts, video_dec_ctx->pix_fmt, video_dec_ctx->width, video_dec_ctx->height);
                    packetCount = 0;
                }
            }
        }
        av_free_packet(&pkt);
    }
    buffer->UpdateEndTimestamp();
    
    return true;
}

bool InputHandler::Open(const char *fname, OutputHandler* out) {
    int i, ret;
    int video_streams = 0, audio_streams = 1;
          
    av_register_all();
    avformat_network_init();
    //av_log_set_level(AV_LOG_DEBUG);
    
    // open input file, and allocate format context
    ret = avformat_open_input(&fmt_ctx, fname, NULL, NULL);
    if (ret < 0) {
        //fprintf(stderr, "Could not open source file %s\n", fname);
        //PAUSE_EXIT(1);
        return false;
    }
    fmt_ctx->flags |= AVFMT_FLAG_GENPTS;
    
    ret = avformat_find_stream_info(fmt_ctx, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_FATAL, "%s: could not find codec parameters\n", fname);
        avformat_close_input(&fmt_ctx);
        return false;
    }
    
    av_dump_format(fmt_ctx, 0, fname, 0);
    
    for (i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            if (fmt_ctx->streams[i]->codec->codec_id == CODEC_ID_MPEG4) {
                bsf = av_bitstream_filter_init("dump_extra");
            } else if (fmt_ctx->streams[i]->codec->codec_id == CODEC_ID_H264) {
                fprintf(stderr, "Found h264 Stream\n");
                //bsf = av_bitstream_filter_init("h264_mp4toannexb");
            } else {
                bsf = NULL;
            }
            video_stream=fmt_ctx->streams[i];
            video_stream_idx = i;
            video_dec_ctx=fmt_ctx->streams[i]->codec;
            video_dec=avcodec_find_decoder(video_dec_ctx->codec_id);
            if(video_dec==NULL) {
                printf("Unsupported Codec error\n");
                return false; // Codec not found
            }
            // Open codec
            if(avcodec_open2(video_dec_ctx, video_dec, NULL)<0) {
                printf("Input Codec Open error\n");
                return false; // Could not open codec
            }

            video_dec_ctx->get_buffer = flow_get_buffer;
            video_dec_ctx->release_buffer = flow_release_buffer;
        }
    }
    m_FPS = ((double)1/video_dec_ctx->ticks_per_frame) * ((double)video_dec_ctx->time_base.den / video_dec_ctx->time_base.num);
    
    double delay = av_q2d(video_stream->codec->time_base);
    printf("DELAY MOFO %g\n", delay);
    frame = avcodec_alloc_frame();
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
     PIX_FMT_RGB24, SWS_BILINEAR,
     NULL, NULL, NULL );

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
    if(m_PictureBuffer) delete m_PictureBuffer;
    avcodec_free_frame(&frame);
    avcodec_free_frame(&frameRGB);
}

bool InputHandler::NextChunk(FrameQueue* fq, PacketQueue* buffer, OutputHandler* out) {
    AVPacket		pkt;
    AVRational		new_tb;
    int			res;
    static int                count = 0;
    int			header_size, header_out;
    int                       frameFinished;
    int                       packetCount = 0;
    double                    pts;
    //fq->swapQueues();
    //buffer->swapQueues();
    for(int frameCount = 0; frameCount < m_MaxFrameCount; ) {
        if (av_read_frame(fmt_ctx, &pkt) >= 0) {
            if (pkt.stream_index == video_stream_idx) {
                pts = 0;
                
                // Save global pts to be stored in pFrame in first call
                global_video_pkt_pts = pkt.pts;
                
                if (avcodec_decode_video2(video_dec_ctx, frame, &frameFinished, &pkt) < 0 ) {
                    fprintf(stderr, "Error decoding packet\n");
                }

                if(pkt.dts == AV_NOPTS_VALUE 
                    && frame->opaque && *(uint64_t*)frame->opaque != AV_NOPTS_VALUE) {
                    pts = *(uint64_t *)frame->opaque;
                } else if(pkt.dts != AV_NOPTS_VALUE) {
                    pts = pkt.dts;
                } else {
                    pts = 0;
                }
                pts *= av_q2d(video_dec_ctx->time_base);

                stream_convert(&pkt);
                if (pkt.pts != AV_NOPTS_VALUE) {
                    pkt.pts = av_rescale_q(pkt.pts, video_stream->time_base, out->video_stream->time_base);
                }
                if (pkt.dts != AV_NOPTS_VALUE) {
                    pkt.dts = av_rescale_q(pkt.dts, video_stream->time_base, out->video_stream->time_base);
                }
                if (pkt.duration != 0) {
                    pkt.duration = av_rescale_q(pkt.duration, video_stream->time_base, out->video_stream->time_base);
                }
                if (av_write_frame(out->fmt_ctx, &pkt) < 0)
                    printf("MITSOS vre eisai!!!!\n");
                BYTE *ptr;
                int chunkSize = avio_close_dyn_buf(out->fmt_ctx->pb,&ptr);
                //printf("Size %d\n",chunkSize);
                // Save global pts to be stored in pFrame in first call
                buffer->ProcessChunk(ptr, chunkSize); 
                av_free(ptr);
                avio_open_dyn_buf(&out->fmt_ctx->pb);
                out->fmt_ctx->pb->max_packet_size = 1472;

                packetCount++;
                if(frameFinished) {
                    frameCount++;
                    pts = SyncVideo(frame, pts);
                    fq->put(frame, pts, video_dec_ctx->pix_fmt, video_dec_ctx->width, video_dec_ctx->height);
                    packetCount = 0;
                }
            }
        }
        av_free_packet(&pkt);
    }
    buffer->UpdateEndTimestamp();
    //printf("Start TS %d End TS %d Diff %d\n", buffer->diff_start_ts, buffer->diff_end_ts, buffer->diff_end_ts-buffer->diff_start_ts);
    return true;
}

static int32_t nal_len_size;
static const uint8_t nalu_header[4] = {0, 0, 0, 1};

int32_t InputHandler::extradata_convert(AVCodecContext *c)
{
    const uint8_t *p;
    int total_size = 0;
    uint8_t *out = NULL;
    uint8_t n;
    
    if (c->extradata_size == 0 || c->extradata == NULL) {
        return 0;
    }
    
    if (*(uint8_t *)c->extradata != 1) {
        return 0;
    }
    
    p = c->extradata + 4;
    nal_len_size = (p[0] & 0x03) + 1;
    n = p[1] & 0x1F;
    p += 2;
    while (n--) {
        uint16_t nal_size;
        
        nal_size = p[0] << 8 | p[1];
        total_size += nal_size + sizeof(nalu_header);
        if (p + 2 + nal_size > c->extradata + c->extradata_size) {
            av_free(out);
            
            return -1;
        }
        out = (uint8_t*)av_realloc(out, total_size);
        if (!out) {
            return -1;
        }
        memcpy(out + total_size - nal_size - sizeof(nalu_header), nalu_header, sizeof(nalu_header));
        memcpy(out + total_size - nal_size, p + 2, nal_size);
        p += 2 + nal_size;
        
        if (n == 0 && p + 2 < c->extradata + c->extradata_size) {
            n = *p++;
        }
    }
    av_free(c->extradata);
    c->extradata = out;
    c->extradata_size = total_size;
    
    return total_size;
}

int32_t InputHandler::stream_convert(AVPacket *pkt)
{
    uint8_t *out = NULL;
    const uint8_t *p = pkt->data;
    int size = 0;
    
    if (nal_len_size == 0) {
        return 0;
    }
    
    while(p + 4 < pkt->data + pkt->size) {
        uint32_t nal_len;
        
        if (nal_len_size != 4) {
            return -1;
        }
        nal_len = p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];
        
        out = (uint8_t *)av_realloc(out, size + nal_len + sizeof(nalu_header));
        memcpy(out + size, nalu_header, sizeof(nalu_header));
        if (p + nal_len_size + nal_len > pkt->data + pkt->size) {
            fprintf(stderr, "NAL Size is broken: %d > %d\n", p - pkt->data + nal_len_size + nal_len, pkt->size);
            nal_len = pkt->data + pkt->size - (p + nal_len_size);
        }
        memcpy(out + size + sizeof(nalu_header), p + nal_len_size, nal_len);
        size += nal_len + sizeof(nalu_header);
        p    += nal_len + nal_len_size;
    }
    if (p != pkt->data + pkt->size) {
        fprintf(stderr, "Strange: %d != %d\n", p - pkt->data, pkt->size);
    }
    av_free(pkt->data);
    
    pkt->data = out;
    pkt->size = size;
    
    return 0;
}

static int open_codec_context(int *stream_idx,
                              AVFormatContext *fmt_ctx, enum AVMediaType type, const char *src_filename)
{
    /*int					ret;
    AVStream			*st;
    AVCodecContext		*dec_ctx = NULL;
    AVCodec				*dec = NULL;
    
	ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not find %s stream in input file '%s'\n",
                av_get_media_type_string(type), src_filename);
        return ret;
    } else {
        *stream_idx = ret;
        st = fmt_ctx->streams[*stream_idx];
        // find decoder for the stream
        dec_ctx = st->codec;
        dec = avcodec_find_decoder(dec_ctx->codec_id);
        if (!dec) {
            fprintf(stderr, "Failed to find %s codec\n",
                    av_get_media_type_string(type));
            return ret;
        }
        if ((ret = avcodec_open2(dec_ctx, dec, NULL)) < 0) {
            fprintf(stderr, "Failed to open %s codec\n",
                    av_get_media_type_string(type));
            return ret;
        }
    }

    return 0;*/
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