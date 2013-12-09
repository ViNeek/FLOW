//
//  fwFrameQueue.h
//  FLOWServer
//
//  Created by Nick Vitsas on 9/17/13.
//  Copyright (c) 2013 Nick Vitsas. All rights reserved.
//

#ifndef __FRAME_QUEUE_HEADER_H__
#define __FRAME_QUEUE_HEADER_H__

#include "../fwUtilities.h"

typedef struct VideoFrame {
    AVFrame*    m_Frame;
    BYTE*       m_Buffer;
    double      m_PTS;
} VideoFrame;

class FrameQueue {
public:
    int m_NextInputFrameIndex, m_NextOutputFrameIndex;
    int m_nb_frames;
    std::vector<VideoFrame>* m_PlayoutFrames;
    std::queue<VideoFrame> m_PlayoutQueue;

    bool    m_Empty;
    wxMutex m_QueueLock;
    double  m_FPS;
    int32_t m_MaxFrameCount;
    
    AVPixelFormat m_PixFmt;
    int m_Width;
    int m_Height;
    FrameQueue();
    FrameQueue(double fps);
    ~FrameQueue();
    
    int put(AVFrame *fr, double pts, AVPixelFormat pix_fmt, int width, int height);
    int get(VideoFrame* vframe);
    int swapQueues();
    void init(int maxFpsCount, AVPixelFormat pix_fmt, int width, int height);
};

#endif // __FRAME_QUEUE_HEADER_H__