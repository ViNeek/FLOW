#include "fwReceiverThread.h"

//
//  fwDisplayThread.cpp
//  FLOWServer
//
//  Created by Nick Vitsas on 9/16/13.
//  Copyright (c) 2013 Nick Vitsas. All rights reserved.
//

#include "fwJobQueue.h"
#include "fwInterface.h"
#include "fwPeerState.h"
#include "fwPRIMEState.h"
#include "fwPacketQueue.h"

ReceiverThread::ReceiverThread(MainFrame* evtHandler, JobQueue* jqueue, PacketQueue* in_buffer, wxDatagramSocket* sock) {
	m_pFrame = evtHandler;
	m_jQueue = jqueue;
          m_InBuffer = in_buffer;
          m_Socket = sock;
}

ReceiverThread::~ReceiverThread() {

}

wxThread::ExitCode ReceiverThread::Entry( ) {
	
	while( OnJob() );
	
	//Clean Up and inform Main thread
	m_pFrame->log("Shit Happen");
	return NULL;
}

bool ReceiverThread::OnJob() {
    Job j = m_jQueue->Pop();
    switch(j.m_Type)
    {
    case Job::THREAD_RECV_CHUNK_ID: 
        return RecvChunk();
        break;
    case Job::THREAD_HOLE_PUNCH_ID: 
        return HolePunch((PRIMEPacket*) j.m_Arg);
        break;
    case Job::THREAD_EXIT_ID: // thread should exit
        delete m_jQueue;
        return false;
        break;
    default:
        delete m_jQueue;
        //Mo way!
        return false;
        break;
    } // switch(j.m_Type)
    
	return false;
}

bool ReceiverThread::HolePunch(PRIMEPacket* pkt) {
    int to_process = -1;
    int lastCount = 0;
    if (m_Socket->IsOk())
    {
        for(int i = 0; i < 5; i++ ) {
	  lastCount = 0;
	  to_process = pkt->m_Length + sizeof(uint32_t);
	  //wxThread::Sleep(20);
	  do {
	      m_Socket->SendTo(pkt->m_Destination, pkt->m_Raw+lastCount,(pkt->m_Length + sizeof(int)) - lastCount);
	      if (m_Socket->Error())
	      {
		wxSocketError e = m_Socket->LastError();
		m_pFrame->log(wxString::Format("Write Error vre %d\n", e));
		//Job j(Job::THREAD_READ_DATA_ID, NULL);
		//m_jQueue->AddNewJob(j);

		delete pkt;

		return true;
	      }
	      lastCount += m_Socket->LastCount();
	      to_process -= m_Socket->LastCount();
	  } while(!m_Socket->Error() && to_process != 0);
        }
        m_pFrame->log(wxString::Format("Punching  %s//%d\n", pkt->m_Destination.IPAddress(), pkt->m_Destination.Service()));
    }
    delete pkt;
    //m_pFrame->log(wxString::Format("Type %d\n", pkt->m_Length));

    //Job j(Job::THREAD_READ_DATA_ID, NULL);
    //m_jQueue->AddNewJob(j);

    return true;
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

bool ReceiverThread::RecvChunk() {
    FLOWPacket pkt;
    int ret = 1;
    int packet_count = 0;
    int header;
    BYTE b[MAX_PAYLOAD_SIZE];

    int lastCount = 0;
    if (m_Socket->IsOk())
    {
        if (!m_Socket->WaitForRead(0,100)) {
            Job j(Job::THREAD_RECV_CHUNK_ID, NULL);
            m_jQueue->AddNewJob(j);

            return true;
        }
        wxIPV4address addr;
        int to_process = MAX_PAYLOAD_SIZE;
        //for(;;) {
	  //lastCount = 0;
	  //to_process = MAX_PAYLOAD_SIZE;
	  //printf("Last Count %d %d\n", m_Socket->LastCount(), m_Socket->Error());
	  //do {
	      m_Socket->RecvFrom(addr, (BYTE*)&b[0]+lastCount,to_process-lastCount);
            
	      //printf("Last Count %d %d\n", m_Socket->LastCount(), m_Socket->Error());
	      if(m_Socket->Error())
		printf("Last Error %d\n", m_Socket->LastError());
	      
	      if(lastCount == 0) {
		//break;
	      }

	      lastCount += m_Socket->LastCount();
	      to_process -= MAX_PAYLOAD_SIZE;

	  //} while (!m_Socket->Error() && to_process != 0);

	  if(lastCount == 0) {
                Job j(Job::THREAD_RECV_CHUNK_ID, NULL);
	      m_jQueue->AddNewJob(j);

	      return true;
            }

	  FLOWPacket pkt;
	  BYTE* current_ptr = &b[0];
	  if((*(current_ptr+1) & 0x7F) == 96) {
	      pkt = parseRTP(current_ptr, m_Socket->LastCount());
	      m_InBuffer->put(pkt);
	      //printf("Packet Seq Num %d\n", pkt.m_SeqNum);
	  }
	  else if(*(current_ptr+1) == 200) {
	      pkt = parseRTCP(current_ptr);
	  } else {
	      printf("Unknown Packet Type\n");
	  }
	  
	  //m_pFrame->log(wxString::Format("Type %d\n", 3));
        //}
    }

    Job j(Job::THREAD_RECV_CHUNK_ID, NULL);
    m_jQueue->AddNewJob(j);

    return true;
}

