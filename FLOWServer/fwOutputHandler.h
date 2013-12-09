#ifndef __OUTPUT_HANDLER_HEADER_H__
#define __OUTPUT_HANDLER_HEADER_H__

#include "../fwUtilities.h"

#include "fwChunk.h"

#define MAX_PAYLOAD_SIZE 1472

class FrameQueue;
class PacketQueue;
class InputHandler;

class OutputHandler 
{
public:

			  OutputHandler();

    bool			  Open(InputHandler* in_h);
    void			  Close();
    Chunk			  NextChunk();
    
    char*                       sdp_desc;
    AVFormatContext*	  fmt_ctx;
    AVOutputFormat*             out_fmt;
    AVCodecContext*		  video_dec_ctx;
    AVCodec*                    video_dec;
    AVStream*		  video_stream;
};

#endif // __OUTPUT_HANDLER_HEADER_H__