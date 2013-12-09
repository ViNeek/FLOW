#ifndef __OUTPUT_HANDLER_HEADER_H__
#define __OUTPUT_HANDLER_HEADER_H__

#include "../fwUtilities.h"
#include "Payload.h"

class OutputHandler {
public:

	OutputHandler();
	bool							Open(const char *fname);
	void							Close();
	void							WriteChunk(Chunk c);

	AVFormatContext*				outctx;

	enum CodecID					video_codec_id;
	enum CodecID					audio_codec_id;
	int								streams;
	int								selected_streams;
	int								vwidth, vheight;
	int								channels;
	int								sample_rate, frame_size;
	AVRational						video_time_base;
	AVRational						audio_time_base;

	char*							output_format;
	char*							output_file;
	int64_t							prev_pts, prev_dts;

private:

	void							format_gen(const Chunk *data);
	void							format_init(const Chunk *c);
};

class ProcessedContent {
public:
	OutputHandler*					m_OutHnd;
	wxMutex*						m_Guard;
	Chunk*							m_Content;
};

#endif // __OUTPUT_HANDLER_HEADER_H__