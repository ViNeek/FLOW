//
//  fwFrameQueue.h
//  FLOWServer
//
//  Created by Nick Vitsas on 9/15/13.
//  Copyright (c) 2013 Nick Vitsas. All rights reserved.
//

#ifndef __PACKET_QUEUE_HEADER_H__
#define __PACKET_QUEUE_HEADER_H__

#include "../fwUtilities.h"

typedef struct FLOWPacket {
    FLOWPacket() : m_Data(NULL) {}
    enum FLOWPacketType{
        NULL_PACKET,
        RTP_PACKET,
        RTCP_PACKET
    };

    int m_Type;
    uint32_t m_Timestamp;
    int32_t m_Size;
    uint32_t m_SeqNum;
    BYTE*   m_Data;
} FLOWPacket;

typedef struct RTPPacket : FLOWPacket {
    bool    m_Marker;
} RTPPacket;

typedef struct RTCPPacket : FLOWPacket {
    int32_t m_Count;
} RTCPPacket;

typedef struct NullPacket : FLOWPacket {
} NullPacket;

class PacketQueue {
public:
    std::vector<FLOWPacket>* m_DiffPackets;
    std::vector<bool>*       m_SwarmState;
    std::vector<FLOWPacket>* m_PlayoutPackets;
    std::vector<FLOWPacket>* m_SwarmPackets;

    uint32_t diff_start_ts, diff_end_ts;
    uint32_t swarm_start_ts, swarm_end_ts;
    uint32_t play_start_ts, play_end_ts;

    uint32_t diff_start_seq, diff_end_seq;
    uint32_t swarm_start_seq, swarm_end_seq;
    uint32_t play_start_seq, play_end_seq;

    int m_NextOutIndex;
    
    wxMutex m_QueueLock;
    
    PacketQueue();
    ~PacketQueue();
    
    void ProcessChunk(BYTE* data, int32_t size);
    int swapQueues();
    void UpdateEndTimestamp();
    int put(FLOWPacket pkt);
    int get(FLOWPacket* pkt);
    PRIMEPacket* getDiffReport();
};

/*
class PacketQueue {
public:
    AVPacketList *m_First, *m_Last;
    int m_nb_packets;
    int m_Size;
    wxMutex m_QueueLock;
    
    PacketQueue();
    ~PacketQueue();
    
    int put(AVPacket *pkt);
    int get(AVPacket *pkt);
};*/

#endif // __PACKET_QUEUE_HEADER_H__
