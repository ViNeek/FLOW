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

#define MAX_PAYLOAD_SIZE 1472

typedef struct FLOWPacket {
    FLOWPacket() : m_Data(NULL) {}
    bool operator== (const FLOWPacket& rhs) const
    {
        return (this->m_SeqNum == rhs.m_SeqNum);
    }
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

typedef struct FLOWPacketTSComparator
{
  bool ascending;
public:
    FLOWPacketTSComparator (const bool& isAscending=false)
    {ascending=isAscending;}
    bool operator() (const FLOWPacket& lhs, const FLOWPacket& rhs) const
    {
        if (ascending) return (lhs.m_Timestamp > rhs.m_Timestamp);
        else return (lhs.m_Timestamp < rhs.m_Timestamp);
    }
} FLOWPacketTSComparator;

typedef struct FLOWPacketSeqComparator
{
  bool ascending;
public:
    FLOWPacketSeqComparator (const bool& isAscending=false)
    {ascending=isAscending;}
    bool operator() (const FLOWPacket& lhs, const FLOWPacket& rhs) const
    {
        if (ascending) return (lhs.m_SeqNum > rhs.m_SeqNum);
        else return (lhs.m_SeqNum < rhs.m_SeqNum);
    }
} FLOWPacketSeqComparator;

//typedef std::priority_queue<FLOWPacket, std::vector<FLOWPacket>, FLOWPacketTSComparator> FLOWTSPriorityQueue;
//typedef std::priority_queue<FLOWPacket, std::vector<FLOWPacket>, FLOWPacketSeqComparator> FLOWSeqPriorityQueue;

typedef std::vector<FLOWPacket> FLOWTSPriorityQueue;
typedef std::vector<FLOWPacket> FLOWSeqPriorityQueue;

class PacketQueue {
public:
    FLOWSeqPriorityQueue* m_DiffPackets;
    FLOWSeqPriorityQueue* m_PlayoutPackets;
    FLOWSeqPriorityQueue* m_SwarmPackets;
    std::vector<bool>*    m_SwarmState;
    std::vector<bool>*    m_DiffState;

    FLOWSeqPriorityQueue* m_DiffRTCPPackets;
    FLOWSeqPriorityQueue* m_PlayoutRTCPPackets;
    FLOWSeqPriorityQueue* m_SwarmRTCPPackets;

    uint32_t diff_start_ts, diff_end_ts;
    uint32_t swarm_start_ts, swarm_end_ts;
    uint32_t play_start_ts, play_end_ts;

    uint32_t diff_start_seq, diff_end_seq;
    uint32_t swarm_start_seq, swarm_end_seq;
    uint32_t play_start_seq, play_end_seq;

    int m_NextOutIndex;
    int m_NextInIndex;

    wxMutex m_QueueLock;
    
    PacketQueue();
    ~PacketQueue();
    
    void ProcessChunk(BYTE* data, int32_t size);
    int swapQueues();
    void UpdateEndTimestamp();
    void UpdateDiffState(uint32_t start, uint32_t end);
    int put(FLOWPacket pkt);
    int get(FLOWPacket* pkt);
    PRIMEPacket* getDiffReport();
    PRIMEPacket* getSwarmReport();
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
