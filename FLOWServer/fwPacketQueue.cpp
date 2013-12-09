//
//  fwFrameQueue.cpp
//  FLOWServer
//
//  Created by Nick Vitsas on 9/15/13.
//  Copyright (c) 2013 Nick Vitsas. All rights reserved.
//

#include "fwPacketQueue.h"
#include "fwOutputHandler.h"

/*
PacketQueue::PacketQueue() {
    m_First = m_Last = NULL;
    m_nb_packets = 0;
    m_Size = 0;
}*/

PacketQueue::PacketQueue() {
    m_DiffPackets = new std::vector<FLOWPacket>;
    m_PlayoutPackets = new std::vector<FLOWPacket>;
    m_SwarmPackets = new std::vector<FLOWPacket>;
    
    m_NextOutIndex = 0;
}

static RTPPacket parseRTP(uint8_t* ptr, uint32_t pkt_size) {
    uint32_t ts;
    uint16_t seq_num;
    bool marker;
    RTPPacket p;
    p.m_Type = FLOWPacket::RTP_PACKET;
    p.m_Size = pkt_size;
    p.m_Data = new BYTE[pkt_size];
    memcpy(p.m_Data, ptr, pkt_size);

    ((uint8_t*)&ts)[0] = *(ptr+7);
    ((uint8_t*)&ts)[1] = *(ptr+6);
    ((uint8_t*)&ts)[2] = *(ptr+5);
    ((uint8_t*)&ts)[3] = *(ptr+4);

    ((uint8_t*)&seq_num)[0] = *(ptr+3);
    ((uint8_t*)&seq_num)[1] = *(ptr+2);

    marker = *(ptr+1) & 0x80;

    //printf("RTPPacket Seq Num %d\n", seq_num);
    //printf("RTPPacket TS %u\n", ts);
    //printf("RTPPacket Marker %u\n", marker);

    p.m_Timestamp = ts;
    p.m_SeqNum = seq_num;
    p.m_Marker = marker;

    return p;
}

static RTCPPacket parseRTCP(uint8_t* ptr) {
    uint16_t length;
    uint32_t ts;
    uint32_t pcount;
    RTCPPacket p;
    p.m_Type = FLOWPacket::RTCP_PACKET;
    
    ((uint8_t*)&length)[0] = *(ptr+3);
    ((uint8_t*)&length)[1] = *(ptr+2);

    ((uint8_t*)&pcount)[0] = *(ptr+23);
    ((uint8_t*)&pcount)[1] = *(ptr+22);
    ((uint8_t*)&pcount)[2] = *(ptr+21);
    ((uint8_t*)&pcount)[3] = *(ptr+20);

    ((uint8_t*)&ts)[0] = *(ptr+19);
    ((uint8_t*)&ts)[1] = *(ptr+18);
    ((uint8_t*)&ts)[2] = *(ptr+17);
    ((uint8_t*)&ts)[3] = *(ptr+16);

    //printf("RTCPPacket Count %d\n", pcount);
    //printf("RTCPPacket Length %d\n", length);
    //printf("RTCPPacket TS %u\n", ts);

    p.m_Size = (length+1) * sizeof(int32_t);
    p.m_Data = new BYTE[p.m_Size];
    memcpy(p.m_Data, ptr, p.m_Size);
    p.m_Timestamp = ts;
    p.m_Count = pcount;

    return p;
}

void PacketQueue::ProcessChunk(BYTE* data, int32_t size) {
    int processed_size = 0;
    int32_t rtcp_size, rtp_count = 0, rtcp_count = 0;
    uint32_t current_size = size;
    BYTE* current_ptr = data;
    RTCPPacket rtcp_pkt;
    RTPPacket rtp_pkt;
    NullPacket null_pkt;

    while(current_size >= MAX_PAYLOAD_SIZE) {
        if((*(current_ptr+1) & 0x7F) == 96) {
            rtp_pkt = parseRTP(current_ptr, MAX_PAYLOAD_SIZE);
            current_size -= MAX_PAYLOAD_SIZE;
            current_ptr += MAX_PAYLOAD_SIZE;
            put(rtp_pkt);
            rtp_count++;
        }
        else if(*(current_ptr+1) == 200) {
            rtcp_pkt = parseRTCP(current_ptr);
            current_size -= rtcp_pkt.m_Size;
            current_ptr += rtcp_pkt.m_Size;
            put(rtcp_pkt);
            rtcp_count++;
        } else {
            printf("Unknown Packet Type\n");
        }
    }
    
    while (current_size > 0) {
        if((*(current_ptr+1) & 0x7F) == 96) {
            rtp_pkt = parseRTP(current_ptr, current_size);
            current_size -= current_size;
            current_ptr += current_size;
            put(rtp_pkt);
            rtp_count++;
        }
        else if(*(current_ptr+1) == 200) {
            rtcp_pkt = parseRTCP(current_ptr);
            current_size -= rtcp_pkt.m_Size;
            current_ptr += rtcp_pkt.m_Size;
            put(rtcp_pkt);
            rtcp_count++;
        } else {
            printf("Unknown Packet Type\n");
        }
    }
    null_pkt.m_Type = FLOWPacket::NULL_PACKET;
    null_pkt.m_Timestamp = rtp_pkt.m_Timestamp;
    null_pkt.m_SeqNum = rtp_pkt.m_SeqNum;
    null_pkt.m_Data = NULL;
    put(null_pkt);

    //printf("%d RTP Packets\n",rtp_count);
}

static bool first_rtp_packet = true;
int PacketQueue::put(FLOWPacket pkt) {
    wxMutexLocker lock(m_QueueLock);
    if(first_rtp_packet) {
        if(pkt.m_Type == FLOWPacket::RTP_PACKET) {
            diff_start_ts = pkt.m_Timestamp;
            diff_start_seq = pkt.m_SeqNum;
            first_rtp_packet = false;
        }
    }

    m_DiffPackets->push_back(pkt);

    return 1;
}

PRIMEPacket* PacketQueue::getDiffReport() {
    wxMutexLocker lock(m_QueueLock);
    PRIME::PRIMEMessage msg;
    msg.set_type(PRIME::PRIMEMessage_MsgType_PRIME_REPORT);
    PRIME::Report rep;
    rep.set_type(PRIME::Report_ReportType_DIFF_REPORT_TYPE);
    PRIME::Report_DiffusionReport diffrep;
    diffrep.set_end_ts(play_end_ts);
    diffrep.set_start_ts(play_start_ts);
    diffrep.set_end_seq(play_end_seq);
    diffrep.set_start_seq(play_start_seq);
    rep.mutable_diff_report()->CopyFrom(diffrep);
    msg.mutable_report()->CopyFrom(rep);

    PRIMEPacket* p = new PRIMEPacket(msg);

    return p;
}

void PacketQueue::UpdateEndTimestamp() {
    wxMutexLocker lock(m_QueueLock);
    diff_end_ts = (m_DiffPackets->end()-1)->m_Timestamp;
    diff_end_seq = (m_DiffPackets->end()-1)->m_SeqNum;
}

int PacketQueue::get(FLOWPacket* pkt) {
    wxMutexLocker lock(m_QueueLock);

    int ret = 0;
    if(m_PlayoutPackets->empty() || (m_NextOutIndex == m_PlayoutPackets->size())) {
        ret = 1;
        return ret;
    }
    //printf("Next %d\n", m_NextOutIndex);
    *pkt = m_PlayoutPackets->at(m_NextOutIndex);
    if(pkt->m_Data) {
        pkt->m_Data = new BYTE[pkt->m_Size];
        memcpy(pkt->m_Data, m_PlayoutPackets->at(m_NextOutIndex).m_Data, pkt->m_Size);
    }
    //printf("THA TO DEIS!!!!!! %d %d %d %d\n", pkt->m_SeqNum, play_end_seq, play_start_seq, swarm_end_seq);
    m_NextOutIndex++;
    /*if(pkt->m_Type == FLOWPacket::NULL_PACKET) {
        m_NextOutIndex++;
        if(m_NextOutIndex == m_PlayoutPackets->size()) {
            ret = 1;
        } else {
            *pkt = m_PlayoutPackets->at(m_NextOutIndex);
            pkt->m_Data = new BYTE[pkt->m_Size];
            memcpy(pkt->m_Data, m_PlayoutPackets->at(m_NextOutIndex).m_Data, pkt->m_Size);
            m_NextOutIndex++;
        }
    } else {
        pkt->m_Data = new BYTE[pkt->m_Size];
        memcpy(pkt->m_Data, m_PlayoutPackets->at(m_NextOutIndex).m_Data, pkt->m_Size);
        m_NextOutIndex++;
    }*/

    if(m_NextOutIndex == m_PlayoutPackets->size())
        ret = 2;

    return ret;
}

int PacketQueue::swapQueues() {
    int ret = 0;
    std::vector<FLOWPacket>* tmp;
    
    wxMutexLocker lock(m_QueueLock);

    tmp = m_PlayoutPackets;
    m_PlayoutPackets = m_SwarmPackets;
    m_SwarmPackets = m_DiffPackets;
    m_DiffPackets = tmp;
    
    play_end_ts = swarm_end_ts;
    play_start_ts = swarm_start_ts;
    swarm_end_ts = diff_end_ts;
    swarm_start_ts = diff_start_ts;
    diff_end_ts = 0;
    diff_start_ts = 0;

    play_end_seq = swarm_end_seq;
    play_start_seq = swarm_start_seq;
    swarm_end_seq = diff_end_seq;
    swarm_start_seq = diff_start_seq;
    diff_end_seq = 0;
    diff_start_seq = 0;

    for(int i = 0; i < m_DiffPackets->size(); i++) {
        if(m_DiffPackets->at(i).m_Data)
            delete m_DiffPackets->at(i).m_Data;
    }

    m_DiffPackets->clear();

    m_NextOutIndex = 0;
    
    first_rtp_packet = true;

    return ret;
}

PacketQueue::~PacketQueue() {
    for(int i = 0; i < m_DiffPackets->size(); i++) {
        if(m_DiffPackets->at(i).m_Data)
            delete m_DiffPackets->at(i).m_Data;
    }
    for(int i = 0; i < m_PlayoutPackets->size(); i++) {
        if(m_PlayoutPackets->at(i).m_Data)
            delete m_PlayoutPackets->at(i).m_Data;
    }
    for(int i = 0; i < m_SwarmPackets->size(); i++) {
        if(m_SwarmPackets->at(i).m_Data)
            delete m_SwarmPackets->at(i).m_Data;
    }
    delete m_SwarmPackets;
    delete m_PlayoutPackets;
    delete m_DiffPackets;
}
/*
int PacketQueue::put(AVPacket *pkt) {
    AVPacketList *pkt_node;
    if(av_dup_packet(pkt) < 0) {
        return -1;
    }
    
    pkt_node = (AVPacketList*)av_malloc(sizeof(AVPacketList));
    if (!pkt_node)
        return -1;
    
    pkt_node->pkt = *pkt;
    pkt_node->next = NULL;
    
    wxMutexLocker lock(m_QueueLock);
    
    if (!m_Last)
        m_First = pkt_node;
    else
        m_Last->next = pkt_node;
    
    m_Last = pkt_node;
    m_nb_packets++;
    m_Size += pkt_node->pkt.size;
    
    return 0;
}

int PacketQueue::get(AVPacket *pkt) {
    AVPacketList *pkt_node;
    int ret = -1;
    
    wxMutexLocker lock(m_QueueLock);
    

    pkt_node = m_First;
    if (pkt_node) {
        m_First = pkt_node->next;
        if (!m_First)
            m_Last = NULL;
        m_nb_packets--;
        m_Size -= pkt_node->pkt.size;
        *pkt = pkt_node->pkt;
        av_free(pkt_node);
        
        ret = 1;
    }
    
    return ret;
}*/