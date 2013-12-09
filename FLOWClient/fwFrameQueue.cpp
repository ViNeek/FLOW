//
//  fwFrameQueue.cpp
//  FLOWServer
//
//  Created by Nick Vitsas on 9/17/13.
//  Copyright (c) 2013 Nick Vitsas. All rights reserved.
//

#include "fwFrameQueue.h"

FrameQueue::FrameQueue() {
    m_nb_frames = 0;
    m_FPS = 0.0f;
    m_Empty = true;
    m_NextInputFrameIndex = 0;
    m_NextOutputFrameIndex = 0;
    m_MaxFrameCount = 0;
    
    m_Height = 0;
    m_Width = 0;
    m_PixFmt = (AVPixelFormat)0;

    m_PlayoutFrames = NULL;
}

FrameQueue::FrameQueue(double fps) {
    m_nb_frames = 0;
    m_FPS = fps;
    m_Empty = true;
    m_NextInputFrameIndex = 0;
    m_NextOutputFrameIndex = 0;
    m_MaxFrameCount = (int32_t)(m_FPS+1);
    m_PlayoutFrames = new std::vector<VideoFrame>(m_MaxFrameCount);
    for(int i = 0; i < m_MaxFrameCount; i++) {
        VideoFrame pframe = {0};
        pframe.m_Frame = avcodec_alloc_frame();
        m_PlayoutFrames->push_back(pframe);
    }
}

void FrameQueue::init(int maxFpsCount, AVPixelFormat pix_fmt, int width, int height) {
    m_nb_frames = 0;
    m_Empty = true;
    m_NextInputFrameIndex = 0;
    m_NextOutputFrameIndex = 0;
    m_MaxFrameCount = maxFpsCount;
    m_Height = height;
    m_Width = width;
    m_PixFmt = pix_fmt;
}

FrameQueue::~FrameQueue() {
    //free frames
    //free vector pointers
    for(int i = 0; i < m_MaxFrameCount; i++) {
        delete m_PlayoutFrames->at(i).m_Buffer;
        avcodec_free_frame(&m_PlayoutFrames->at(i).m_Frame);
    }
    delete m_PlayoutFrames;
}

int FrameQueue::put(AVFrame *frame, double pts, AVPixelFormat pix_fmt, int width, int height) {
    wxMutexLocker lock(m_QueueLock);
    int ret = 0;
    VideoFrame pframe = {0};
    //printf("\n\n\n\nSIZE                                    %d\n\n\n\n\n", m_PlayoutQueue.size());
    pframe.m_Frame = avcodec_alloc_frame();
    pframe.m_PTS = pts;
    int numBytes=avpicture_get_size(pix_fmt, width, height);
    pframe.m_Buffer=new BYTE[numBytes];
    avpicture_fill((AVPicture *)pframe.m_Frame, pframe.m_Buffer, pix_fmt, width, height);
    av_picture_copy((AVPicture*)pframe.m_Frame,(AVPicture*)frame, pix_fmt, width, height);

    m_PlayoutQueue.push(pframe);
    
    return ret;
}

int FrameQueue::swapQueues() {
    wxMutexLocker lock(m_QueueLock);
    int ret = 0;
    
    m_NextInputFrameIndex = 0;
    m_NextOutputFrameIndex = 0;
    
    return ret;
}

int FrameQueue::get(VideoFrame* vframe) {
    wxMutexLocker lock(m_QueueLock);
    int ret = 0;
    //printf("getting\n");
    if(m_PlayoutQueue.empty()) {
        ret = 1;
        return ret;
    }

    *vframe = m_PlayoutQueue.front();

    m_PlayoutQueue.pop();
    /*AVFrameList *frame_node;
    int ret = -1;
    
    wxMutexLocker lock(m_QueueLock);
    
    
    frame_node = m_First;
    if (frame_node) {
        m_First = frame_node->next;
        if (!m_First)
            m_Last = NULL;
        m_nb_frames--;
        frame = frame_node->frame;
        av_free(frame_node);
        
        ret = 1;
    }*/
    
    return ret;
}
