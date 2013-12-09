//
//  fwFrameQueue.cpp
//  FLOWServer
//
//  Created by Nick Vitsas on 9/17/13.
//  Copyright (c) 2013 Nick Vitsas. All rights reserved.
//

#include "fwFrameQueue.h"

//
//  fwFrameQueue.cpp
//  FLOWServer
//
//  Created by Nick Vitsas on 9/15/13.
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
    
    m_PlayoutFrames = NULL;
    m_DiffuseFrames = NULL;
    m_SwarmFrames = NULL;
}

FrameQueue::FrameQueue(double fps) {
    m_nb_frames = 0;
    m_FPS = fps;
    m_Empty = true;
    m_NextInputFrameIndex = 0;
    m_NextOutputFrameIndex = 0;
    m_MaxFrameCount = (int32_t)(m_FPS+0.5);
    m_PlayoutFrames = new std::vector<VideoFrame>(m_MaxFrameCount);
    m_DiffuseFrames = new std::vector<VideoFrame>(m_MaxFrameCount);
    m_SwarmFrames = new std::vector<VideoFrame>(m_MaxFrameCount);
    for(int i = 0; i < m_MaxFrameCount; i++) {
        VideoFrame pframe = {0};
        VideoFrame sframe = {0};
        VideoFrame dframe = {0};
        pframe.m_Frame = avcodec_alloc_frame();
        sframe.m_Frame = avcodec_alloc_frame();
        dframe.m_Frame = avcodec_alloc_frame();
        m_PlayoutFrames->push_back(pframe);
        m_DiffuseFrames->push_back(dframe);
        m_SwarmFrames->push_back(sframe);
    }
}

void FrameQueue::init(double fps, AVPixelFormat pix_fmt, int width, int height) {
    m_nb_frames = 0;
    m_FPS = fps;
    m_Empty = true;
    m_NextInputFrameIndex = 0;
    m_NextOutputFrameIndex = 0;
    m_MaxFrameCount = (int32_t)(m_FPS+0.5);
    m_PlayoutFrames = new std::vector<VideoFrame>;
    m_DiffuseFrames = new std::vector<VideoFrame>;
    m_SwarmFrames = new std::vector<VideoFrame>;
    for(int i = 0; i < m_MaxFrameCount; i++) {
        VideoFrame pframe = {0};
        VideoFrame sframe = {0};
        VideoFrame dframe = {0};
        pframe.m_Frame = avcodec_alloc_frame();
        sframe.m_Frame = avcodec_alloc_frame();
        dframe.m_Frame = avcodec_alloc_frame();
        int numBytes=avpicture_get_size(pix_fmt, width, height);
        pframe.m_Buffer=new BYTE[numBytes];
        sframe.m_Buffer=new BYTE[numBytes];
        dframe.m_Buffer=new BYTE[numBytes];
        avpicture_fill((AVPicture *)pframe.m_Frame, pframe.m_Buffer, pix_fmt, width, height);
        avpicture_fill((AVPicture *)sframe.m_Frame, sframe.m_Buffer, pix_fmt, width, height);
        avpicture_fill((AVPicture *)dframe.m_Frame, dframe.m_Buffer, pix_fmt, width, height);
    
        m_PlayoutFrames->push_back(pframe);
        m_DiffuseFrames->push_back(dframe);
        m_SwarmFrames->push_back(sframe);
        //printf("Diffuse Size %d\n", m_DiffuseFrames->size());
        //printf("Swarm Size %d\n", m_SwarmFrames->size());
        //printf("Playout Size %d\n", m_PlayoutFrames->size());
    }

}

FrameQueue::~FrameQueue() {
    //free frames
    //free vector pointers
    for(int i = 0; i < m_MaxFrameCount; i++) {
        delete m_DiffuseFrames->at(i).m_Buffer;
        delete m_PlayoutFrames->at(i).m_Buffer;
        delete m_SwarmFrames->at(i).m_Buffer;
        avcodec_free_frame(&m_DiffuseFrames->at(i).m_Frame);
        avcodec_free_frame(&m_PlayoutFrames->at(i).m_Frame);
        avcodec_free_frame(&m_SwarmFrames->at(i).m_Frame);
    }
    delete m_DiffuseFrames;
    delete m_PlayoutFrames;
    delete m_SwarmFrames;
}

int FrameQueue::put(AVFrame *frame, double pts, AVPixelFormat pix_fmt, int width, int height) {
    int ret = 0;
    
    //avcodec_get_frame_defaults(m_DiffuseFrames->at(m_NextInputFrameIndex).m_Frame);
    m_DiffuseFrames->at(m_NextInputFrameIndex).m_PTS = pts;
    
    AVFrame* s = m_DiffuseFrames->at(m_NextInputFrameIndex).m_Frame;
    av_picture_copy((AVPicture*)s,(AVPicture*)frame, pix_fmt, width, height);

    m_NextInputFrameIndex++;
    if(m_NextInputFrameIndex == m_MaxFrameCount)
        ret = 1;
    
    /*AVFrameList *frame_node;
    
    frame_node = (AVFrameList*)av_malloc(sizeof(AVFrameList));
    if (!frame_node)
        return -1;
    
    frame_node->frame = frame;
    frame_node->next = NULL;
    
    wxMutexLocker lock(m_QueueLock);
    
    if (!m_Last)
        m_First = frame_node;
    else
        m_Last->next = frame_node;
    
    m_Last = frame_node;
    m_nb_frames++;*/
    
    return ret;
}

int FrameQueue::swapQueues() {
    int ret = 0;
    std::vector<VideoFrame>* tmp;
    
    tmp = m_PlayoutFrames;
    m_PlayoutFrames = m_SwarmFrames;
    m_SwarmFrames = m_DiffuseFrames;
    m_DiffuseFrames = tmp;
    
    m_NextInputFrameIndex = 0;
    m_NextOutputFrameIndex = 0;
    
    return ret;
}

int FrameQueue::get(VideoFrame* vframe) {
    int ret = 0;
    
    if (m_PlayoutFrames->size() <= m_NextOutputFrameIndex) {
        ret = 1;
        return ret;
    }
    vframe->m_PTS = m_PlayoutFrames->at(m_NextOutputFrameIndex).m_PTS;
    vframe->m_Frame = m_PlayoutFrames->at(m_NextOutputFrameIndex).m_Frame;
    
    m_NextOutputFrameIndex++;
    if(m_NextOutputFrameIndex == m_MaxFrameCount)
        ret = 1;
    
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
