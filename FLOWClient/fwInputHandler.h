#ifndef __INPUT_HANDLER_HEADER_H__
#define __INPUT_HANDLER_HEADER_H__

#include "../fwUtilities.h"

#define HEADER_REFRESH_PERIOD 50

class FrameQueue;
class PacketQueue;

class InputHandler 
{
public:

			  InputHandler(const char *sdp, int width, int height, AVPixelFormat pix_fmt);

    bool			  Open();
    void			  Close();
    double                      SyncVideo(AVFrame* fr, double pts);
    bool                        NextChunk(FrameQueue* fq);

    PacketQueue*                in_buffer;
    char                        *sdp_desc;
    AVFormatContext*	  fmt_ctx;
    AVCodecContext*		  video_dec_ctx;
    AVCodec*                    video_dec;
    AVStream*		  video_stream;
    int                         video_stream_idx;
    AVFrame*                    frame;
    AVFrame*                    frameRGB;
    AVIOContext*                sdp_io_context;
    AVIOContext*                io_context;
    BYTE*                       io_buffer;
    struct SwsContext*          sws_ctx;
    struct SwsContext*          sws_ctx_move;
    
    AVBitStreamFilterContext*	  bsf;

    double                      m_FrameTimer;
    double                      m_LastPTS;
    double                      m_LastDelay;
    double                      m_VideoClock;
    double                      m_FPS;
    int                         m_MaxFrameCount;

    int			  m_Width;
    int			  m_Height;
    AVPixelFormat		  m_PixFmt;

    int                         m_NumBytes;
    BYTE*                       m_PictureBuffer;
};

#endif // __INPUT_HANDLER_HEADER_H__