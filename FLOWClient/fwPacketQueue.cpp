#include "fwPacketQueue.h"

/*
PacketQueue::PacketQueue() {
    m_First = m_Last = NULL;
    m_nb_packets = 0;
    m_Size = 0;
}*/

PacketQueue::PacketQueue() {
    //m_DiffPackets = new FLOWSeqPriorityQueue(FLOWPacketSeqComparator(true));
    //m_PlayoutPackets = new FLOWSeqPriorityQueue(FLOWPacketSeqComparator(true));
    //m_SwarmPackets = new FLOWSeqPriorityQueue(FLOWPacketSeqComparator(true));

    m_DiffPackets = new FLOWSeqPriorityQueue();
    m_PlayoutPackets = new FLOWSeqPriorityQueue();
    m_DiffState = new std::vector<bool>(1024);
    m_SwarmPackets = new FLOWSeqPriorityQueue();
    m_SwarmState = new std::vector<bool>(1024);

    play_end_ts = 0;
    play_start_ts = 0;
    swarm_end_ts = 0;
    swarm_start_ts = 0;
    diff_end_ts = 0;
    diff_start_ts = 0;

    play_end_seq = 0;
    play_start_seq = 0;
    swarm_end_seq = 0;
    swarm_start_seq = 0;
    diff_end_seq = 0;
    diff_start_seq = 0;

    m_NextOutIndex = 0;
    m_NextInIndex = 0;
}

void PacketQueue::ProcessChunk(BYTE* data, int32_t size) {
    int processed_size = 0;
    int32_t rtcp_size, rtp_count = 0, rtcp_count = 0;
    uint32_t current_size = size;
    BYTE* current_ptr = data;
    RTCPPacket rtcp_pkt;
    RTPPacket rtp_pkt;
    NullPacket null_pkt;
}

static bool first_rtp_packet = true;
int PacketQueue::put(FLOWPacket pkt) {
    wxMutexLocker lock(m_QueueLock);
    //printf("THA TO DEIS!!!!!! %d %d %d %d\n", pkt.m_SeqNum, diff_end_seq, diff_start_seq, swarm_end_seq);
    if(pkt.m_SeqNum > swarm_end_seq) {
        //m_DiffPackets->push(pkt);
        m_DiffPackets->push_back(pkt);
        //m_DiffState->at(pkt.m_SeqNum - diff_start_seq) = true;
    } else {
        printf("THA TO DEIS!!!!!!\n");
        //m_SwarmPackets->push(pkt);
        //m_SwarmState->at(pkt.m_SeqNum - swarm_start_seq) = true;
        m_SwarmPackets->push_back(pkt);
    }

    return 1;
}

PRIMEPacket* PacketQueue::getDiffReport() {
    /*PRIME::PRIMEMessage msg;
    msg.set_type(PRIME::PRIMEMessage_MsgType_PRIME_REPORT);
    PRIME::Report rep;
    rep.set_type(PRIME::Report_ReportType_DIFF_REPORT_TYPE);
    PRIME::Report_DiffusionReport diffrep;
    diffrep.set_end_ts(play_end_ts);
    diffrep.set_start_ts(play_start_ts);
    rep.mutable_diff_report()->CopyFrom(diffrep);
    msg.mutable_report()->CopyFrom(rep);

    PRIMEPacket* p = new PRIMEPacket(msg);
    */
    PRIME::PRIMEMessage msg;
    PRIMEPacket* p = new PRIMEPacket(msg);
    return p;
}

PRIMEPacket* PacketQueue::getSwarmReport() {
    wxMutexLocker lock(m_QueueLock);
    //if(m_SwarmPackets->size() == 0) {
    //    printf("PROBLEMO!!!!!!!\n");
    //}
    PRIME::PRIMEMessage msg;
    msg.set_type(PRIME::PRIMEMessage_MsgType_PRIME_REPORT);
    PRIME::Report rep;
    rep.set_type(PRIME::Report_ReportType_SWARM_REPORT_TYPE);
    PRIME::Report_SwarmingReport swarmrep;
    swarmrep.set_end_seq(swarm_end_seq);
    swarmrep.set_start_seq(swarm_start_seq);
    for(int i = 0; i < m_SwarmState->size(); i++)
        swarmrep.add_state(m_SwarmState->at(i));
    rep.mutable_swarm_report()->CopyFrom(swarmrep);
    msg.mutable_report()->CopyFrom(rep);
    int trues = 0; 
    int falses = 0;
    //printf("Diff %u %u %u %u\n", swarm_end_seq, swarm_start_seq, diff_end_seq, diff_start_seq);
    for(int i = 0; i < (swarm_end_seq-swarm_start_seq); i++) {
        if(m_SwarmState->at(i) == false) {
	  falses++;
        } else {
	  trues++;
        }
    }

    printf("Trues %d Falses %d\n", trues, falses);
    /*wxString str;
    std::sort (m_SwarmPackets->begin(), m_SwarmPackets->end(), FLOWPacketSeqComparator(false)); 
    int start_seq = m_SwarmPackets->at(0).m_SeqNum; 
    int end_seq = start_seq;
    str.Append(wxString::Format("%d-", start_seq));
    for( int i = 1; i < m_SwarmPackets->size(); i++ ) {
        if( ( m_SwarmPackets->at(i).m_SeqNum - end_seq ) >= 2 ) {
	  start_seq = m_SwarmPackets->at(i).m_SeqNum;
	  str.Append(wxString::Format("%d ", end_seq));
	  str.Append(wxString::Format("%d-", start_seq));
        }
        end_seq = m_SwarmPackets->at(i).m_SeqNum;
    }
    str.Append(wxString::Format("%d ", end_seq));
    wxPrintf("String Rep : %s\n", str);*/
    PRIMEPacket* p = new PRIMEPacket(msg);
    
    return p;
}

void PacketQueue::UpdateEndTimestamp() {
    //diff_end_ts = (m_DiffPackets->end()-1)->m_Timestamp;
}

void PacketQueue::UpdateDiffState(uint32_t start, uint32_t end) {
    wxMutexLocker lock(m_QueueLock);
    diff_end_seq = end;
    diff_start_seq = start;
}

int PacketQueue::get(FLOWPacket* pkt) {
    wxMutexLocker lock(m_QueueLock);
    /*
    int ret = 0;
    if(m_PlayoutPackets->empty()) {
        ret = 1;
        return ret;
    }
    //printf("Next %d\n", m_NextOutIndex);
    *pkt = m_PlayoutPackets->top();
    if(pkt->m_Data) {
        //pkt->m_Data = new BYTE[pkt->m_Size];
        //memcpy(pkt->m_Data, m_PlayoutPackets->top().m_Data, pkt->m_Size);
    }
    m_NextOutIndex++;
    m_PlayoutPackets->pop();

    if(m_PlayoutPackets->empty())
        ret = 2;
        */

    int ret = 0;
    if(m_PlayoutPackets->empty() || m_NextOutIndex == m_PlayoutPackets->size()) {
        ret = 1;
        return ret;
    }
    //printf("Next %d\n", m_NextOutIndex);
    *pkt = m_PlayoutPackets->at(m_NextOutIndex);
    if(pkt->m_Data) {
        //pkt->m_Data = new BYTE[pkt->m_Size];
        //memcpy(pkt->m_Data, m_PlayoutPackets->top().m_Data, pkt->m_Size);
    }
    m_NextOutIndex++;
    //m_PlayoutPackets->pop();

    if(m_NextOutIndex == m_PlayoutPackets->size())
        ret = 2;

    return ret;
}

int PacketQueue::swapQueues() {
    int ret = 0;
    FLOWSeqPriorityQueue* tmp;
    std::vector<bool> tmpb;

    wxMutexLocker lock(m_QueueLock);

    //std::sort (m_DiffPackets->begin(), m_DiffPackets->end(), FLOWPacketSeqComparator(false)); 
    //m_DiffPackets->erase( std::unique( m_DiffPackets->begin(), m_DiffPackets->end() ), m_DiffPackets->end() );
    tmp = m_PlayoutPackets;
    m_PlayoutPackets = m_SwarmPackets;
    m_SwarmPackets = m_DiffPackets;
    m_DiffPackets = tmp;
    
    //*m_SwarmState = *m_DiffState;

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

    m_DiffPackets->clear();
    for(int i = 0; i < m_DiffPackets->size(); i++) {
       //if(m_DiffPackets->at(i).m_Data)
            //delete m_DiffPackets->at(i).m_Data;
    }
    
    for(int i = 0; i < m_DiffState->size(); i++)
        m_DiffState->at(i) = false;

    m_NextOutIndex = 0;
    m_NextInIndex = 0;

    first_rtp_packet = true;
    
    if(play_end_seq == 0 && play_start_seq == 0) {
        ret = 1;
    } else {
        for(int i = 0; i < m_SwarmState->size(); i++) {
	  m_SwarmState->at(i) = false;
        }
        for(int i = 0; i < m_SwarmPackets->size(); i++) {
	  int index = m_SwarmPackets->at(i).m_SeqNum - (swarm_start_seq);
	  m_SwarmState->at(index) = true;
        }
        //printf("John");
    }

    return ret;
}

PacketQueue::~PacketQueue() {
    int diffSize = m_DiffPackets->size();
    int swarmSize = m_SwarmPackets->size();
    int playSize = m_PlayoutPackets->size();

    for(int i = 0; i < diffSize; i++) {
        if(m_DiffPackets->at(i).m_Data)
            delete m_DiffPackets->at(i).m_Data;
    }
    for(int i = 0; i < playSize; i++) {
        if(m_PlayoutPackets->at(i).m_Data)
            delete m_PlayoutPackets->at(i).m_Data;

    }
    for(int i = 0; i < swarmSize; i++) {
        if(m_SwarmPackets->at(i).m_Data)
            delete m_SwarmPackets->at(i).m_Data;
    }
    delete m_SwarmPackets;
    delete m_SwarmState;
    delete m_PlayoutPackets;
    delete m_DiffPackets;
    delete m_DiffState;
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