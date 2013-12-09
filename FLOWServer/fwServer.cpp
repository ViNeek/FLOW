//
//  flow.cpp
//  FLOW
//
//  Created by Nick Vitsas on 9/5/13.
//  Copyright (c) 2013 Nick Vitsas. All rights reserved.
//

#include "fwServer.h"

/*
 * Copyright (c) 2003 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * @file
 * libavformat API example.
 *
 * @example libavformat/output-example.c
 * Output a media file in any supported libavformat format.
 * The default codecs are used.
 */

#define _USE_MATH_DEFINES

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef _DEBUG
#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#if _MSC_VER
#define snprintf _snprintf
#define inline __inline
#define PAUSE_EXIT(x) getchar();exit((x))
#define __STDC_CONSTANT_MACROS
#else
#define PAUSE_EXIT(x) exit((x))
#define BOOL int
#define TRUE 1
#define FALSE 0
#endif

#define STRING_BOOL(expression) (expression) ? "TRUE" : "FALSE"

static const BOOL software_streaming = TRUE;

#include "../fwUtilities.h"

#include "fwInterface.h"

class PRIMEServer : public wxApp
{
	MainFrame *m_frame;
    
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(PRIMEServer);

bool PRIMEServer::OnInit()
{
#ifdef _DEBUG
    Utilities::SetStdOutToNewConsole();
#endif

    PRIME::Reply a;
    
    m_frame = new MainFrame( (wxFrame *)NULL , -1, wxString::FromUTF8("FLOW Server") );
    m_frame->SetIcon(wxICON(FLOW));
    m_frame->Show( true );

    a.set_type(PRIME::Reply_ReplyType_SPEED_TEST);
    int size = a.ByteSize();
    m_frame->log(wxString::Format("Size %d", size));
	
    wxString str;
    str.Format("%d",wxThread::GetCPUCount());
    m_frame->log(str);
    //printf("\nValue %d\n", a.type());
    if(wxSocketBase::Initialize()){
        wxString t;
        t = wxString::Format("%d",a.type());
        wxString s(wxT("Socket Base Initialized\n"+t));
        m_frame->log(s);
    } else {
        //m_frame->C3OOutputText->AppendText(wxT("Socket Base NOT Initialized... Exiting!\n"));
    }
    
    return true;
}

#if 0
/*
#include <stdint.h>

extern "C" {
#include <libavutil/avutil.h>
#include <libavutil/mathematics.h>
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/time.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}*/

void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) {
    FILE *pFile;
    char szFilename[32];
    int  y;
    
    // Open file
    sprintf(szFilename, "frame%d.ppm", iFrame);
    pFile=fopen(szFilename, "wb");
    if(pFile==NULL)
        return;
    
    // Write header
    fprintf(pFile, "P6\n%d %d\n255\n", width, height);
    
    // Write pixel data
    for(y=0; y<height; y++)
        fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);
    
    // Close file
    fclose(pFile);
}

#if 0
static int read_frame(AVFormatContext *in_ctx)
{
    AVPacket    pkt;
    AVStream    *st;
    int         res;
    int64_t     send_time;
    
    if(in_ctx == NULL) {
        return -2;
    }
    
    res = av_read_frame(in_ctx, &pkt);
    if (res < 0) {
        return -2;
    }
    
    if (pkt.stream_index != 0) {
        return -1;
    }
    
    st = in_ctx->streams[pkt.stream_index];
    printf("%d: %Ld %Ld\n", pkt.stream_index, pkt.pts, pkt.dts);
    if (pkt.dts == AV_NOPTS_VALUE) {
        fprintf(stderr, "WTF??? Unknown DTS???\n");
        
        return -2;
    }
    //if (rtp_c->out_s[pkt.stream_index]->streams[0]->codec->codec_id == CODEC_ID_H264) {
    //  stream_convert(&pkt);
    //}
    send_time = av_rescale_q(pkt.dts, st->time_base, AV_TIME_BASE_Q);
    time_base_convert(&pkt, st, rtp_c->out_s[pkt.stream_index]->streams[0]);
    if (st->start_time != AV_NOPTS_VALUE) {
        send_time -= av_rescale_q(st->start_time,
			    st->time_base, AV_TIME_BASE_Q);
    }
    dbg_print("Read Frame: %Ld (%Ld)\n", send_time, pkt.dts);
    rtp_c->queued_frames[pkt.stream_index]++;
    
    send_time += rtp_c->first_time;
    
    return process_frame(rtp_c->out_s[pkt.stream_index], &pkt, send_time);
}
#endif

static int nal_len_size;
static const uint8_t nalu_header[4] = {0, 0, 0, 1};

static int extradata_convert(AVCodecContext *c)
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

static int stream_convert(AVPacket *pkt)
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
        
        out = (uint8_t*)av_realloc(out, size + nal_len + sizeof(nalu_header));
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

void printByte(uint8_t* ptr) {
    for(int i = 7; i >= 0; i--)
        printf("%d",((*ptr)>>i) & 1);
    printf("\n");
}

void parseRTP(uint8_t* ptr, uint32_t stream_size) {
    uint16_t ptr16;
    uint8_t* current_ptr = ptr;
    uint16_t seq_num;
    uint32_t current_size = stream_size;
    uint32_t count = 1;
    printf("Stream Size %d\n", stream_size);
    while(current_size > 1460) {
        if((*(current_ptr+1) & 0x7F) == 96) {
	  ((uint8_t*)&seq_num)[0] = *(current_ptr+3);
	  ((uint8_t*)&seq_num)[1] = *(current_ptr+2);
	  printf("RTP packet %d\n", seq_num);
	  current_ptr += 1460;
	  current_size -= 1460;
	  printf("%d %d\n",current_size, stream_size);
	  count++;
	  //parseRTP(ptr);
        } else {
	  printf("Bullshit\n");
        }
    }
    ((uint8_t*)&seq_num)[0] = *(current_ptr+3);
    ((uint8_t*)&seq_num)[1] = *(current_ptr+2);
    printf("RTP packet %d\n", seq_num);
    printf("%d RTP Packets\n",count);
}

void parseRTCP(uint8_t* ptr, uint32_t stream_size) {
    uint16_t ptr16;
    uint8_t* current_ptr = ptr;
    uint16_t seq_num;
    uint32_t current_size = stream_size;
    uint32_t count = 1;
    
    ((uint8_t*)&ptr16)[0] = *(ptr+3);
    ((uint8_t*)&ptr16)[1] = *(ptr+2);
    
    current_ptr += ptr16*4 + 4;
    current_size -= ptr16*4 + 4;
    
    while(current_size > 1460) {
        if((*(current_ptr+1) & 0x7F) == 96) {
	  ((uint8_t*)&seq_num)[0] = *(current_ptr+3);
	  ((uint8_t*)&seq_num)[1] = *(current_ptr+2);
	  printf("RTP packet %d\n", seq_num);
	  current_ptr += 1460;
	  current_size -= 1460;
	  printf("%d %d\n",current_size, stream_size);
	  count++;
	  //parseRTP(ptr);
        } else {
	  printf("Bullshit\n");
        }
    }
    ((uint8_t*)&seq_num)[0] = *(current_ptr+3);
    ((uint8_t*)&seq_num)[1] = *(current_ptr+2);
    printf("RTP packet %d\n", seq_num);
    printf("%d RTP Packets\n",count);
}

void parseStream(uint8_t* ptr, uint32_t stream_size) {
    if((*(ptr+1) & 0x7F) == 96) {
        printf("RTP packet\n");
        parseRTP(ptr, stream_size);
    }
    else if(*(ptr+1) == 200) {
        printf("RTCP Packet\n");
        parseRTCP(ptr, stream_size);
    } else {
        printf("Unknown Packet Type\n");
    }
}

int main(int argc, char **argv)
{
    /*
     int i;
     char    b[40];
     char    c[21];
     HANDLE hConsole;
     int k;
     
     #ifdef _DEBUG
     _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
     #endif
     
     for(i = 0; i < sizeof(b); ++i )
     {
     b[i] = ' ';
     }
     b[255] = '\0';
     
     system("color 5");
     for(i = 0; i < 20; ++i) {
     c[i] = '>';
     c[i+1] = '\0';
     printf("Progress |%s%*s|\r",c,19-i,&"");
     Sleep(100);
     //printf("%s\r", b);
     }
     printf("\n");
     printf("sizeof(structa_t) = %d\n", sizeof(structa_t));
     printf("sizeof(structb_t) = %d\n", sizeof(structb_t));
     printf("sizeof(structc_t) = %d\n", sizeof(structc_t));
     printf("sizeof(structd_t) = %d\n", AV_TIME_BASE);
     
     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
     
     // you can loop k higher to see more color choices
     for(k = 1; k < 255; k++)
     {
     // pick the colorattribute k you want
     SetConsoleTextAttribute(hConsole, k);
     printf("%d I want to be nice today!",k);
     }*/
    
    AVFormatContext             *in_ctx = NULL, *out_ctx = NULL;
    AVInputFormat               *file_iformat = NULL;
    AVOutputFormat              *out_fmt = NULL;
    AVFrame                     *frame = NULL, *frameRGB = NULL;
    AVStream                    *st = NULL;
    AVCodecContext              *codec_ctx = NULL, *pCodecCtx = NULL;
    AVCodec                     *codec = NULL, *pCodec = NULL;
    AVCodec                     dummy_codec = {0};
    AVPacket                    pkt, p;
    AVBitStreamFilterContext    *bsf = NULL;
    struct SwsContext           *sws_ctx = NULL;
    BOOL                        tom = TRUE;
    char                        b[1024];
    int                         err, i, ret, frameFinished, numBytes;
    const char                  *src_filename = "final.mp4";
    int64_t                     timestamp;
    uint8_t                     buf[128];
    uint8_t                     *buffer = NULL;
    int                         video_stream_idx = -1;
    int                         audio_stream_idx = -1;
    FILE*                       sdp_file;
    
#ifdef _DEBUG
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
    
    /* register all formats and codecs */
    av_register_all();
    avformat_network_init();
    av_log_set_level(AV_LOG_DEBUG);

    /* open input file, and allocate format context */
    ret = avformat_open_input(&in_ctx, src_filename, NULL, NULL);
    if (ret < 0) {
        fprintf(stderr, "Could not open source file %s\n", src_filename);
        PAUSE_EXIT(1);
    }
    in_ctx->flags |= AVFMT_FLAG_GENPTS;

    ret = avformat_find_stream_info(in_ctx, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_FATAL, "%s: could not find codec parameters\n", src_filename);
        avformat_close_input(&in_ctx);
        PAUSE_EXIT(1);
    }
    
    av_dump_format(in_ctx, 0, src_filename, 0);
    
    for (i = 0; i < in_ctx->nb_streams; i++) {
        AVStream        *st_ptr;
        AVCodecContext  *coctx_ptr;
        
        if (in_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
	  if (in_ctx->streams[i]->codec->codec_id == CODEC_ID_MPEG4) {
	      bsf = av_bitstream_filter_init("dump_extra");
	  } else if (in_ctx->streams[i]->codec->codec_id == CODEC_ID_H264) {
	      fprintf(stderr, "Found h264 Stream\n");
	      bsf = av_bitstream_filter_init("h264_mp4toannexb");
	  } else {
	      bsf = NULL;
	  }
	  pCodecCtx=in_ctx->streams[i]->codec;
	  pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
	  if(pCodec==NULL) {
	      fprintf(stderr, "Unsupported codec!\n");
	      return -1; // Codec not found
	  }
	  // Open codec
	  if(avcodec_open2(pCodecCtx, pCodec, NULL)<0)
	      return -1; // Could not open codec
	  
	  out_ctx = avformat_alloc_context();
	  out_fmt = av_guess_format("rtp", NULL, NULL);
	  if (!out_fmt) {
	      fprintf(stderr, "Unable for find the RTP format for output\n");
	      avformat_close_input(&in_ctx);
	      PAUSE_EXIT(1);
	  }
	  out_ctx->oformat = out_fmt;
	  //out_ctx->flags |= AVFMT_FLAG_NONBLOCK;
	  
	  st = avformat_new_stream(out_ctx, 0);
	  if (!st) {
	      fprintf(stderr, "Cannot allocate stream\n");
	      avformat_close_input(&in_ctx);
	      PAUSE_EXIT(1);
	  }
	  
	  dummy_codec.type = in_ctx->streams[i]->codec->codec_type;
	  codec_ctx = st->codec;
	  avcodec_get_context_defaults3(codec_ctx, &dummy_codec);
	  avcodec_open2(codec_ctx, NULL, NULL);
	  codec_ctx->codec_type = in_ctx->streams[i]->codec->codec_type;
	  
	  /* FIXME: global headers stuff... */
	  
	  snprintf(out_ctx->filename, sizeof(out_ctx->filename), "rtp://%s:%d", "127.0.0.1", 55444);
	  
	  /* open the UDP sockets for RTP and RTCP */
	  if (!software_streaming) {
	      printf("Distant Connection\n");
	      ret = avio_open(&out_ctx->pb, out_ctx->filename, AVIO_FLAG_WRITE);
	      if (ret < 0) {
		fprintf(stderr, "Cannot open '%s'\n", out_ctx->filename);
		avformat_close_input(&in_ctx);
		PAUSE_EXIT(1);
	      }
	  } else {
	      ret = avio_open_dyn_buf(&out_ctx->pb);
	      out_ctx->pb->max_packet_size = 1460;
	      printf("MAX packet size = %d\n",out_ctx->pb->max_packet_size);
	  }
	  st_ptr = in_ctx->streams[i];
	  coctx_ptr = st_ptr->codec;
	  
	  codec_ctx->codec_id = coctx_ptr->codec_id;
	  codec_ctx->codec_type = coctx_ptr->codec_type;
	  
	  if(!codec_ctx->codec_tag) {
	      codec_ctx->codec_tag = coctx_ptr->codec_tag;
	  }
	  codec_ctx->bit_rate = coctx_ptr->bit_rate;
	  printf("\n\n\n\nFIRE!!!!! %d %d\n\n\n\n", codec_ctx->profile, codec_ctx->level);
	  if(coctx_ptr->extradata_size) {
	      codec_ctx->extradata = (uint8_t*)av_malloc(coctx_ptr->extradata_size);
	      memcpy(codec_ctx->extradata, coctx_ptr->extradata, coctx_ptr->extradata_size);
	  } else {
	      codec_ctx->extradata = NULL;
	  }
	  
	  codec_ctx->extradata_size = coctx_ptr->extradata_size;
	  /* FIXME: ExtraData ??? */
	  if (codec_ctx->codec_id == CODEC_ID_H264) {
	      printf("BINGO\n");
	      extradata_convert(codec_ctx);
	  }
	  
	  if(out_ctx->oformat->flags & AVFMT_GLOBALHEADER)
	      codec_ctx->flags |= CODEC_FLAG_GLOBAL_HEADER;
	  
	  if(av_q2d(coctx_ptr->time_base) > av_q2d(st_ptr->time_base) && av_q2d(st_ptr->time_base) < 1.0/1000) {
	      codec_ctx->time_base = coctx_ptr->time_base;
	  } else {
	      codec_ctx->time_base = st_ptr->time_base;
	  }
	  
	  switch(codec_ctx->codec_type) {
	      case AVMEDIA_TYPE_AUDIO:
		codec_ctx->sample_rate = coctx_ptr->sample_rate;
		codec_ctx->time_base.den = 1;
		codec_ctx->time_base.num = coctx_ptr->sample_rate;
		codec_ctx->channels = coctx_ptr->channels;
		codec_ctx->frame_size = coctx_ptr->frame_size;
		codec_ctx->block_align= coctx_ptr->block_align;
		
		break;
	      case AVMEDIA_TYPE_VIDEO:
		//printf("Pixel Format %d\n", coctx_ptr->pix_fmt);
		codec_ctx->pix_fmt = coctx_ptr->pix_fmt;
		codec_ctx->width = coctx_ptr->width;
		codec_ctx->height = coctx_ptr->height;
		codec_ctx->has_b_frames = coctx_ptr->has_b_frames;
		
		break;
	      default:
		fprintf(stderr, "Strange Codec Type %d\n", codec_ctx->codec_type);
		PAUSE_EXIT(1);
	  }
	  
	  
	  ret = avformat_write_header(out_ctx, NULL);
	  if (ret < 0) {
	      fprintf(stderr, "Cannot Initialize output stream %d\n", i);
	      //close_output(rtp_c->out_s[i]);
	      
	      continue;
	  }
	  av_dump_format(out_ctx, i, out_ctx->filename, 1);
        }
    }
    
    frame = avcodec_alloc_frame();
    frameRGB = avcodec_alloc_frame();
    
    // Determine required buffer size and allocate buffer
    numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,
			  pCodecCtx->height);
    buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
    printf("Allocated %d", numBytes);
    
    sws_ctx = sws_getContext (
     pCodecCtx->width,
     pCodecCtx->height,
     pCodecCtx->pix_fmt,
     pCodecCtx->width,
     pCodecCtx->height,
     PIX_FMT_RGB24,
     SWS_BILINEAR,
     NULL,
     NULL,
     NULL
     );
    
    // Assign appropriate parts of buffer to image planes in pFrameRGB
    // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
    // of AVPicture
    avpicture_fill((AVPicture *)frameRGB, buffer, PIX_FMT_RGB24,
		 pCodecCtx->width, pCodecCtx->height);
    
    av_sdp_create(&out_ctx,1,b,1024);
    printf("SDP : \n%s", b);
    sdp_file = fopen("rtp.sdp","w");
    fprintf(sdp_file, "%s",b);
    fclose(sdp_file);

    i = 0;
    av_init_packet(&pkt);
    av_init_packet(&p);
    printf("Payload Size %d\n", *(uint8_t *)out_ctx->streams[0]->codec->extradata != 1);
    while (av_read_frame(in_ctx, &pkt) >= 0) {
        if (pkt.stream_index == 0) {
	  int res;
	  uint8_t *ptr;
	  uint16_t ptr16;
	  
	  if (avcodec_decode_video2(pCodecCtx, frame, &frameFinished, &pkt) < 0 ) {
	      fprintf(stderr, "Error decoding packet\n");
	  }
	   
	  /* if(frameFinished) {
	  // Convert the image from its native format to RGB
	  sws_scale
	  (
	  sws_ctx,
	  (uint8_t const * const *)frame->data,
	  frame->linesize,
	  0,
	  pCodecCtx->height,
	  frameRGB->data,
	  frameRGB->linesize
	  );
	   
	  // Save the frame to disk
	  if(++i<=5)
	      SaveFrame(frameRGB, pCodecCtx->width, pCodecCtx->height, i);
	  }*/
	  printf("PTS %lld DTS%lld\n",pkt.pts,pkt.dts);
	  printf("Got frame %s %d %s\n",STRING_BOOL(frameFinished), pkt.size, STRING_BOOL(pkt.flags & AV_PKT_FLAG_KEY));
	  //break;
	  /*ret = av_bitstream_filter_filter(bsf,
	   in_ctx->streams[pkt.stream_index]->codec,
	   NULL, &p.data, &p.size,
	   pkt.data, pkt.size, pkt.flags & AV_PKT_FLAG_KEY);
	   if(ret > 0) {
	   av_free_packet(&pkt);
	   p.destruct = av_destruct_packet;
	   } else if (ret < 0) {
	   fprintf(stderr, "%s failed for stream %d, codec %s: ",
	   bsf->filter->name,
	   pkt.stream_index,
	   in_ctx->streams[pkt.stream_index]->codec->codec->name);
	   fprintf(stderr, "%d\n", ret);
	   }
	   pkt = p;*/
	  
	  stream_convert(&pkt);
	  printf("pkt size %d %d\n",pkt.size, pkt.flags);
	  av_usleep(4000000);
	  
	  if (av_write_frame(out_ctx, &pkt) < 0)
	      printf("MITSOS eisai!!!!\n");
	  
	  int written_size = avio_close_dyn_buf(out_ctx->pb,&ptr);
	  printf("Written Size %d\n", written_size);
	  ((uint8_t*)&ptr16)[0] = *(ptr+2);
	  ((uint8_t*)&ptr16)[1] = *(ptr+3);
	  printf("CC adsasd%d\n", ptr16 );
	  printByte(ptr);
	  printByte(ptr+1);
	  //printf("Second Byte %d\n", *(ptr+1));
	  
	  parseStream(ptr, written_size);
	  
	  printf("Version %d\n",(*(ptr) & 0xC0) >> 6);
	  printf("Padding %d\n",(*(ptr) & 0x20) >  0);
	  printf("Ext %d\n",(*(ptr) & 0x10) >  0);
	  printf("CC %d\n",(*(ptr) & 0xF));
	  printf("Marker %d\n",(*(ptr+1) & 0x80) > 0);
	  printf("Type %u\n",(*(ptr+1)));
	  printf("Seq %d\n",(*((uint16_t*)((uint8_t*)ptr+2))));
	  ret = avio_open_dyn_buf(&out_ctx->pb);
	  out_ctx->pb->max_packet_size = 1514;
        }
        
        av_free_packet(&pkt);
    }
#if 0
    av_sdp_create(&out_ctx,1,b,1024);
    printf("SDP : \n%s", b);
    sdp_file = fopen("./rtp.sdp","wb");
    fwrite(b,strlen(b),1,sdp_file);
    fclose(sdp_file);
    av_init_packet(&pkt);
    av_init_packet(&p);
    while (av_read_frame(in_ctx, &pkt) >= 0) {
        if (pkt.stream_index == 0) {
	  //AVPacket new_pkt = pkt;
	  int res;
	  uint8_t *ptr;
	  uint16_t ptr16;
	  /*
	   res = av_bitstream_filter_filter(bsf,
	   in_ctx->streams[pkt.stream_index]->codec,
	   NULL, &new_pkt.data, &new_pkt.size,
	   pkt.data, pkt.size, pkt.flags & AV_PKT_FLAG_KEY);
	   if(res > 0) {
	   av_free_packet(&pkt);
	   new_pkt.destruct = av_destruct_packet;
	   } else if (res < 0) {
	   fprintf(stderr, "%s failed for stream %d, codec %s: ",
	   bsf->filter->name,
	   pkt.stream_index,
	   in_ctx->streams[pkt.stream_index]->codec->codec->name);
	   fprintf(stderr, "%d\n", res);
	   }
	   pkt = new_pkt;*/
	  printf("pkt size %d\n",pkt.size);
	  Sleep(1000);
	  if (av_write_frame(out_ctx, &pkt) < 0)
	      printf("MITSOS eisai!!!!\n");
	  //av_free_packet(&p);
#if 1
	  printf("Size %d\n",avio_close_dyn_buf(out_ctx->pb,&ptr));
	  ((uint8_t*)&ptr16)[0] = *(ptr+3);
	  ((uint8_t*)&ptr16)[1] = *(ptr+2);
	  printf("CC %d\n", ptr16 );
	  printf("Version %d\n",(*(ptr) & 0xC0) >> 6);
	  printf("Padding %d\n",(*(ptr) & 0x20) >  0);
	  printf("Ext %d\n",(*(ptr) & 0x10) >  0);
	  printf("CC %d\n",(*(ptr) & 0xF));
	  printf("Marker %d\n",(*(ptr+1) & 0x80) > 0);
	  printf("Type %u\n",(*(ptr+1)));
	  printf("Seq %d\n",(*((uint16_t*)((uint8_t*)ptr+2))));
	  ret = avio_open_dyn_buf(&out_ctx->pb);
	  out_ctx->pb->max_packet_size = 1514;
#endif
        }
        av_free_packet(&pkt);
    }
#endif
#if 0
    oc = avformat_alloc_context();
    if (!oc)
    {
        fprintf(stderr, "Could not allocate AVFormatContext for RTP\n");
        PAUSE_EXIT(1);
    }
    
    _snprintf(oc->filename, sizeof(oc->filename), "rtp://%s:%d", "192.168.1.56", 30000);
    // get the output format
    out_fmt = av_guess_format("rtp", NULL, NULL);
    if (!out_fmt)
    {
        fprintf(stderr, "Could not allocate AVOutputFormat for RTP\n");
        PAUSE_EXIT(1);
    }
    oc->oformat = out_fmt;
    
    if (avio_open(&(oc->pb), oc->filename, AVIO_FLAG_WRITE) < 0)
    {
        fprintf(stderr, "Could not allocate RTP output stream\n");
        PAUSE_EXIT(1);
    }
    
    ret = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not find %% stream in input file '%s'\n",
	      AVMEDIA_TYPE_VIDEO, src_filename);
        PAUSE_EXIT(1);
    }
    video_stream_idx = ret;
    st = ic->streams[video_stream_idx];
    /* find decoder for the stream */
    dec_ctx = st->codec;
    dec = avcodec_find_decoder(dec_ctx->codec_id);
    if (!dec) {
        fprintf(stderr, "Failed to find %d codec\n",
	      AVMEDIA_TYPE_VIDEO);
    }
    if ((ret = avcodec_open2(dec_ctx, dec, NULL)) < 0) {
        fprintf(stderr, "Failed to open %d codec\n",
	      AVMEDIA_TYPE_VIDEO);
    }
    if (oc->oformat->flags & AVFMT_GLOBALHEADER)
        dec_ctx->flags |= CODEC_FLAG_GLOBAL_HEADER;
    
    
    frame = avcodec_alloc_frame();
    if (!frame) {
        fprintf(stderr, "Could not allocate frame\n");
        ret = AVERROR(ENOMEM);
        PAUSE_EXIT(1);
    }
    /* initialize packet, set data to NULL, let the demuxer fill it */
    av_init_packet(&pkt);
    av_init_packet(&p);
    pkt.data = NULL;
    pkt.size = 0;
    
    st = avformat_new_stream(oc, dec);
    if (!st)
    {
        fprintf(stderr, "Could not allocate H264 stream");
        PAUSE_EXIT(1);
    }
    
    // initalize codec
    dec_ctx = st->codec;
    printf("Codec Type %d %s\n",st->codec==NULL, st->codec->codec_name);
    dec_ctx->codec_id = CODEC_ID_H264;
    dec_ctx->codec_type = AVMEDIA_TYPE_VIDEO;
    dec_ctx->bit_rate = ic->streams[video_stream_idx]->codec->bit_rate;
    printf("Bit rate %d\n", ic->streams[video_stream_idx]->codec->bit_rate);
    dec_ctx->width = ic->streams[video_stream_idx]->codec->width;
    dec_ctx->height = ic->streams[video_stream_idx]->codec->height;
    dec_ctx->time_base.den = 25;
    dec_ctx->time_base.num = 1;
    
    //dec = avcodec_find_decoder(ic->streams[video_stream_idx]->codec->codec_id);
    if (!dec)
    {
        fprintf(stderr, "Could not allocate codec for %d", ic->streams[video_stream_idx]->codec->codec_id);
        PAUSE_EXIT(1);
    }
    
    //if (avcodec_open2(dec_ctx, dec, NULL) < 0) {
    //    fprintf(stderr, "Could not open codec\n");
    //    PAUSE_EXIT(1);
    //}
    malloc(100000);
    printf("Num of streams %d\n",oc->oformat->priv_class==NULL);
    
    // write the header
    avformat_write_header(oc, NULL);
    av_sdp_create(&oc,1,b,1024);
    printf("SDP : \n%s", b);
    sdp_file = fopen("./rtp.sdp","wb");
    fwrite(b,strlen(b),1,sdp_file);
    fclose(sdp_file);
    while (av_read_frame(ic, &pkt) >= 0) {
        if (pkt.stream_index == video_stream_idx) {
	  if(tom){
	      //printf("Values %d %d %d %d\n", oc->streams[pkt.stream_index]!=NULL, pkt.dts, pkt.pts, pkt.size);
	      tom = FALSE;
	  }
	  Sleep(33);
	  p.data = pkt.data;
	  p.flags |= AV_PKT_FLAG_KEY;
	  p.pts = p.dts = 0;
	  if (av_write_frame(oc, &p) < 0)
	      printf("MITSOS eisai!!!!\n");
	  av_free_packet(&p);
        }
        av_free_packet(&pkt);
    }
    
    //printf("\nQueried %d\n",oc->oformat->query_codec(AV_CODEC_ID_H264,FF_COMPLIANCE_NORMAL));
    av_dump_format(oc, 0, oc->filename, 1);
#endif
    
    getchar();
    ret = av_write_trailer(out_ctx);
    
    if(out_ctx) {
        if (software_streaming) {
	  uint8_t *dummy;
	  avio_close_dyn_buf(out_ctx->pb, &dummy);
	  av_free(dummy);
        } else {
	  avio_close(out_ctx->pb);
        }
    }
    
    if(buffer) av_free(buffer);
    //av_free(frameRGB);
    
    // Free the YUV frame
    //av_free(frame);
    
    if(codec_ctx) avcodec_close(codec_ctx);
    if(pCodecCtx) avcodec_close(pCodecCtx);
    if(bsf) av_bitstream_filter_close(bsf);
    if(out_ctx) avformat_free_context(out_ctx);
    if(in_ctx) avformat_close_input(&in_ctx);
    
    //av
    return 0;
}


/*
#include "wx/wx.h"

class MyApp: public wxApp
{
    virtual bool OnInit();
    
    wxFrame *frame;
};

IMPLEMENT_APP(MyApp)


bool MyApp::OnInit()
{
    frame = new wxFrame(NULL, -1,  wxT("Hello wxWidgets"), wxPoint(50,50), wxSize(800,600));
    frame->Show();
    return true;
}
*/

#endif