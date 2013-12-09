#include "fwOutputHandler.h"

#include "fwPacketQueue.h"
#include "fwFrameQueue.h"
#include "fwInputHandler.h"

static int32_t extradata_convert(AVCodecContext *c);

OutputHandler::OutputHandler() {
    fmt_ctx = NULL;
    out_fmt = NULL;
    video_dec_ctx = NULL;
    video_dec = NULL;
    video_stream = NULL;
    sdp_desc = NULL;
}


bool OutputHandler::Open(InputHandler* in_h) {
    fmt_ctx = avformat_alloc_context();
    out_fmt = av_guess_format("rtp", NULL, NULL);
    if (!out_fmt) {
        return false;
    }
    fmt_ctx->oformat = out_fmt;
    //fmt_ctx->flags |= AVFMT_FLAG_NONBLOCK;
            
    video_stream = avformat_new_stream(fmt_ctx, 0);
    if (!video_stream) {
        return false;
    }
    
    AVCodec dummy_codec;
    dummy_codec.type = in_h->video_stream->codec->codec_type;
    dummy_codec.id = in_h->video_dec_ctx->codec_id;
    video_dec_ctx = video_stream->codec;
    //printf("Is NULL %d", dummy_codec.type == AVMEDIA_TYPE_VIDEO);
    avcodec_get_context_defaults3(video_dec_ctx, in_h->video_dec_ctx->codec);
    //avcodec_open2(video_dec_ctx, NULL, NULL);
    video_dec_ctx->codec_type = in_h->video_stream->codec->codec_type;
    
    // FIXME: global headers stuff... 

    snprintf(fmt_ctx->filename, sizeof(fmt_ctx->filename), "rtp://%s:%d", "127.0.0.1", 55444);

    int ret = avio_open_dyn_buf(&fmt_ctx->pb);
    if (ret < 0) {
        printf("AVIO Problem\n");
        return false;
    }
    fmt_ctx->pb->max_packet_size = MAX_PAYLOAD_SIZE;
    
    AVStream* st_ptr = in_h->video_stream;
    AVCodecContext* coctx_ptr = st_ptr->codec;

    video_dec_ctx->codec_id = coctx_ptr->codec_id;
    video_dec_ctx->codec_type = coctx_ptr->codec_type;
    
    if(!video_dec_ctx->codec_tag) {
        video_dec_ctx->codec_tag = coctx_ptr->codec_tag;
    }
    video_dec_ctx->bit_rate = coctx_ptr->bit_rate;
    if(coctx_ptr->extradata_size) {
        video_dec_ctx->extradata = (uint8_t*)av_malloc(coctx_ptr->extradata_size);
        memcpy(video_dec_ctx->extradata, coctx_ptr->extradata, coctx_ptr->extradata_size);
    } else {
        video_dec_ctx->extradata = NULL;
    }
    
    video_dec_ctx->extradata_size = coctx_ptr->extradata_size;
    // FIXME: ExtraData ???
    if (video_dec_ctx->codec_id == CODEC_ID_H264) {
        in_h->extradata_convert(video_dec_ctx);
    }

    if(fmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
        video_dec_ctx->flags |= CODEC_FLAG_GLOBAL_HEADER;

    if(av_q2d(coctx_ptr->time_base) > av_q2d(st_ptr->time_base) && av_q2d(st_ptr->time_base) < 1.0/1000) {
        video_dec_ctx->time_base = coctx_ptr->time_base;
    } else {
        video_dec_ctx->time_base = st_ptr->time_base;
    }
    
    switch(video_dec_ctx->codec_type) {
    case AVMEDIA_TYPE_AUDIO:
        video_dec_ctx->sample_rate = coctx_ptr->sample_rate;
        video_dec_ctx->time_base.den = 1;
        video_dec_ctx->time_base.num = coctx_ptr->sample_rate;
        video_dec_ctx->channels = coctx_ptr->channels;
        video_dec_ctx->frame_size = coctx_ptr->frame_size;
        video_dec_ctx->block_align= coctx_ptr->block_align;

        break;
    case AVMEDIA_TYPE_VIDEO:
        //printf("Pixel Format %d\n", coctx_ptr->pix_fmt);
        video_dec_ctx->pix_fmt = coctx_ptr->pix_fmt;
        video_dec_ctx->width = coctx_ptr->width;
        video_dec_ctx->height = coctx_ptr->height;
        video_dec_ctx->has_b_frames = coctx_ptr->has_b_frames;

        break;
    default:
        return false;
    }
    BYTE* ptr;
    ret = avformat_write_header(fmt_ctx, NULL);
    if (ret < 0) {
        return false;
    }
    
    sdp_desc = new char[1024];
    av_sdp_create(&fmt_ctx,1,sdp_desc,1024);
    printf("SDP File \n%s %d", sdp_desc, video_dec_ctx->extradata);
    //av_dump_format(fmt_ctx, 0, fmt_ctx->filename, 1);
}

void OutputHandler::Close() {
    if(fmt_ctx) {
        BYTE* ptr;
        av_write_trailer(fmt_ctx);
        avio_close_dyn_buf(fmt_ctx->pb,&ptr);
        av_free(ptr);
    }
    if(video_dec_ctx) avcodec_close(video_dec_ctx);

    if(fmt_ctx)
        avformat_free_context(fmt_ctx);
  
    if(sdp_desc) delete sdp_desc;
    //if(fmt_ctx) avformat_close_input(&fmt_ctx);
    //if(sws_ctx) sws_freeContext(sws_ctx);
    //if(bsf) av_bitstream_filter_close(bsf);
}

Chunk OutputHandler::NextChunk() {
    Chunk c;
    return c;
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