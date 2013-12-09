#ifndef __INPUT_HANDLER_HEADER_H__
#define __INPUT_HANDLER_HEADER_H__

#include "../fwUtilities.h"

#include "fwChunk.h"

#define HEADER_REFRESH_PERIOD 50

class FrameQueue;
class PacketQueue;
class OutputHandler;
class PacketBuffer;

class InputHandler 
{
public:

			  InputHandler();

    bool			  Open(const char *fname, OutputHandler* out);
    void			  Close();
    bool			  NextChunk(FrameQueue* fq, PacketQueue* buffer, OutputHandler* out);
    double                      SyncVideo(AVFrame* fr, double pts);
    bool                        InitStream(FrameQueue* fq, PacketQueue* buffer, OutputHandler* out);
    int32_t                     stream_convert(AVPacket *pkt);
    int32_t                     extradata_convert(AVCodecContext *c);

    char*                       sdp_desc;
    AVFormatContext*	  fmt_ctx;
    AVCodecContext*		  video_dec_ctx;
    AVCodec*                    video_dec;
    AVStream*		  video_stream;
    int                         video_stream_idx;
    AVFrame*                    frame;
    AVFrame*                    frameRGB;
    struct SwsContext*          sws_ctx;
    struct SwsContext*          sws_ctx_move;
    
    AVBitStreamFilterContext*	  bsf;

    double                      m_FrameTimer;
    double                      m_LastPTS;
    double                      m_LastDelay;
    double                      m_VideoClock;
    double                      m_FPS;
    int                         m_MaxFrameCount;

    int                         m_NumBytes;
    BYTE*                       m_PictureBuffer;
};

#endif // __INPUT_HANDLER_HEADER_H__