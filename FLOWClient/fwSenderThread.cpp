#include "fwSenderThread.h"

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

SenderThread::SenderThread(MainFrame* evtHandler, JobQueue* jqueue, PacketQueue* out_buffer, int session_type, wxDatagramSocket* sock) {
	m_pFrame = evtHandler;
	m_jQueue = jqueue;
          m_OutBuffer = out_buffer;
          m_SessionType = session_type;
          m_Socket = sock;
}
SenderThread::~SenderThread() {

}
wxThread::ExitCode SenderThread::Entry( ) {
	
	while( OnJob() );
	
	//Clean Up and inform Main thread
	m_pFrame->log("Shit Happen");
	return NULL;
}

bool SenderThread::OnJob() {
    Job j = m_jQueue->Pop();
    switch(j.m_Type)
    {
        case Job::THREAD_SEND_CHUNK_WITH_POLICY_ID:
	  return SendChunkWithPolicy((Policy*)j.m_Arg);
	  break;
        case Job::THREAD_SEND_CHUNK_ID: 
            return SendChunk();
            break;
        case Job::THREAD_EXIT_ID: // thread should exit
	  printf("GAVDGAVDGASVDGASVDGASVDGVASDGVAGDVGADVADVASDVGASDV\n");
            delete m_jQueue;
	  m_Socket->Destroy();
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

bool SenderThread::SendChunkWithPolicy(Policy* pol) {
    /*std::vector<wxIPV4address> snap = m_pFrame->m_PRIMEState->getDiffSnapshot();
    //printf("LUDA!!!!!\N");
    if(snap.size() == 0)
        return true;

    if(pol->m_PeerIndex >= snap.size()) {
        pol->m_PeerIndex = 0;
    }
    FLOWPacket pkt;
    int ret = 1;
    int packet_count = 0;
    //printf("Stuff\n");
    //printf("Index 2%d\n", m_OutBuffer->m_NextOutIndex);
    ret = m_OutBuffer->get(&pkt);
    while((ret != 1)) {
        //printf("Return %d\n", ret);
        if(pkt.m_Type == FLOWPacket::NULL_PACKET) {
	  ret = m_OutBuffer->get(&pkt);
	  continue;
        }
        packet_count++;
        //wxThread::Sleep(1);
        //printf("Index %d %d\n", mpla, pkt.m_SeqNum);
        if (!m_Socket->WaitForWrite(0,30)) {
	  Job j(Job::THREAD_SEND_CHUNK_ID, NULL);
	  m_jQueue->AddNewJob(j);

	  return true;
        }
        m_Socket->SendTo(snap[pol->m_PeerIndex], pkt.m_Data, pkt.m_Size);
        if(m_Socket->Error()){
	  wxSocketError e = m_Socket->LastError();
	  //printf("Packet Count %d\n", packet_count);
	  if(e == wxSOCKET_WOULDBLOCK) {
	      //printf("Packet Count %d\n", packet_count);
	  }
        }
        //delete pkt.m_Data;
        if(packet_count > pol->m_MaxPacketCount) {
	  pol->m_PeerIndex++;
	  Job j(Job::THREAD_SEND_CHUNK_WITH_POLICY_ID, pol);
	   m_jQueue->AddNewJob(j);

	  break;
        }
        
        ret = m_OutBuffer->get(&pkt);
    }
    //wxThread::Sleep(30);
    //Job j(Job::THREAD_SEND_CHUNK_ID, NULL);
    //m_jQueue->AddNewJob(j);
    */
    return true;
}

bool SenderThread::SendChunk() {
    /*std::vector<wxIPV4address> snap = m_pFrame->m_PRIMEState->getDiffSnapshot();
    FLOWPacket pkt;
    int ret = 1;
    int packet_count = 0;
    //printf("Stuff\n");
    //printf("Index 2%d\n", m_OutBuffer->m_NextOutIndex);
    ret = m_OutBuffer->get(&pkt);
    while((ret != 1)) {
        //printf("Return %d\n", ret);
        if(pkt.m_Type == FLOWPacket::NULL_PACKET) {
	  ret = m_OutBuffer->get(&pkt);
	  continue;
        }
        packet_count++;
        //wxThread::Sleep(1);
        for(int i = 0; i < snap.size(); i++ ) {
            //printf("Index %d %d\n", mpla, pkt.m_SeqNum);
	  if (!m_Socket->WaitForWrite(0,30)) {
	      Job j(Job::THREAD_SEND_CHUNK_ID, NULL);
	      m_jQueue->AddNewJob(j);

	      return true;
	  }
	  m_Socket->SendTo(snap[i], pkt.m_Data, pkt.m_Size);
	  if(m_Socket->Error()){
	      wxSocketError e = m_Socket->LastError();
	      //printf("Packet Count %d\n", packet_count);
	      if(e == wxSOCKET_WOULDBLOCK) {
		//printf("Packet Count %d\n", packet_count);
	      }
	  }
        }
        //delete pkt.m_Data;
        if(packet_count > 11111)
	  break;
        
        ret = m_OutBuffer->get(&pkt);
    }
    //wxThread::Sleep(30);
    Job j(Job::THREAD_SEND_CHUNK_ID, NULL);
    m_jQueue->AddNewJob(j);
    */
    return true;
}

