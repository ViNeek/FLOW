#ifndef __PAYLOAD_HEADER_H__
#define __PAYLOAD_HEADER_H__


/*
 *  Copyright (c) 2010 Luca Abeni
 *
 *  This is free software; see gpl-3.0.txt
 */

#include "Chunk.h"
#include "IntegerCoding.h"

#define MAX_STREAMS 20
#define AUDIO_PAYLOAD_HEADER_SIZE 1 + 1 + 2 + 2 + 1 // 1 Frame type + 1 channels 2 sample rate + 2 frame size + 1 number of frames
#define VIDEO_PAYLOAD_HEADER_SIZE 1 + 2 + 2 + 2 + 2 + 1 // 1 Frame type + 2 width + 2 height + 2 frame rate num + 2 frame rate den + 1 number of frames
#define FRAME_HEADER_SIZE (3 + 4 + 1)	// 3 Frame size + 4 PTS + 1 DeltaTS
#define UDP_CHUNK_HEADER_SIZE (2 + 1)   // 2 size + 1 stream

static inline void frame_header_parse(const uint8 *data, int *size, int64 *pts, int64 *dts)
{
  int i;

  *size = 0;
/* FIXME: Maybe this can use int_coding? */

  for (i = 0; i < 3; i++) {
    *size = *size << 8;
    *size |= data[i];
  }
  *dts = int_rcpy(data + 3);
  if (data[7] != 255) {
    *pts = *dts + data[7];
  } else {
    *pts = -1;
  }
}

static FAST_EXEC void audio_payload_header_parse(const uint8 *data, uint8 *codec, uint8 *ch, int *sr, int *fs)
{
  *codec = data[0];
  *ch = data[1];
  *sr = int16_rcpy(data + 2);
  *fs = int16_rcpy(data + 4);
}

static FAST_EXEC void video_payload_header_parse(const uint8 *data, uint8 *codec, int *width, int *height, int *frame_rate_n, int *frame_rate_d)
{
  *codec = data[0];
  *width = int16_rcpy(data + 1);
  *height = int16_rcpy(data + 3);
  *frame_rate_n = int16_rcpy(data + 5);
  *frame_rate_d = int16_rcpy(data + 7);
}

static FAST_EXEC void audio_payload_header_write(uint8 *data, uint8 codec, unsigned int channels, unsigned int sample_rate, unsigned int frame_size)
{
  data[0] = codec;
  data[1] = channels;
  int16_cpy(data + 2, sample_rate);
  int16_cpy(data + 4, frame_size);
}

static FAST_EXEC void video_payload_header_write(uint8 *data, uint8 codec, int width, int height, int num, int den)
{
  data[0] = codec;
  int16_cpy(data + 1, width);
  int16_cpy(data + 3, height);
  int16_cpy(data + 5, num);
  int16_cpy(data + 7, den);
}

static FAST_EXEC void frame_header_write(uint8 *data, int size, int32 pts, int32 dts)
{
  data[0] = size >> 16;
  data[1] = size >> 8;
  data[2] = size & 0xFF;
  int_cpy(data + 3, dts);
  if (pts != -1) {
    data[7] = (pts - dts) & 0xFF;
  } else {
    data[7] = 255;
  }
}

static FAST_EXEC void udp_chunk_header_parse(uint8 *data, int *size, int *stream)
{
  *size = int16_rcpy(data);
  *stream = data[2]; 
}

static FAST_EXEC void udp_chunk_header_write(uint8 *data, int size, uint8 stream)
{
  int16_cpy(data, size);
  data[2] = stream;
}

#endif // __PAYLOAD_HEADER_H__
