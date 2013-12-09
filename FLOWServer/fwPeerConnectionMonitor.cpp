//
//  fwDisplayThread.cpp
//  FLOWServer
//
//  Created by Nick Vitsas on 9/16/13.
//  Copyright (c) 2013 Nick Vitsas. All rights reserved.
//

#include "fwPeerConnectionMonitor.h"
#include "fwJobQueue.h"
#include "fwInterface.h"
#include "fwPeerState.h"

PeerConnectionMonitor::PeerConnectionMonitor(MainFrame* evtHandler, PeerState* peer) {
    m_pFrame = evtHandler;
    m_Peer = peer;
    m_Peer->m_Socket->Notify(false);
    m_Peer->m_Socket->SetFlags(wxSOCKET_NOWAIT);
    //m_Peer->m_Socket->SetFlags(wxSOCKET_WAITALL);
    m_Peer->m_Socket->GetPeer(m_Peer->m_Addr);
}

PeerConnectionMonitor::~PeerConnectionMonitor() {

}

wxThread::ExitCode PeerConnectionMonitor::Entry( ) {
	
    while( OnJob() );

    return NULL;
}

bool PeerConnectionMonitor::OnJob() {
    Job j = m_Peer->m_Queue->Pop();
    switch(j.m_Type)
    {
    case Job::THREAD_READ_DATA_ID: // thread should exit
        return ReadData();
        break;
    case Job::THREAD_WRITE_DATA_ID: // process a job that terminates with an error
        return WriteData((PRIME::PRIMEMessage*) j.m_Arg);
        break;
    case Job::THREAD_EXIT_ID:
        //Friendly goodbye probably
        printf("READY TO RUMBLE\n");
        m_Peer->m_Socket->Destroy();
        delete m_Peer->m_Queue;
        delete m_Peer;
        return false;
        break;
    default:
        //Mo way!
        return false;
        break;
    } // switch(j.m_Type)
    
    return false;
}

bool PeerConnectionMonitor::WriteData(PRIME::PRIMEMessage* msg) {
    int to_process = -1;
    int lastCount = 0;
    wxIPV4address addr;
    m_Peer->m_Socket->GetPeer(addr);
    m_pFrame->log(wxString::Format("Sending a Message of type %d to %s\n", msg->type(), addr.IPAddress()));
    if (m_Peer->m_Socket->IsConnected())
    {
        m_pFrame->log(wxT("inner"));
        PRIMEPacket pack(*msg);
        to_process = pack.m_Length + sizeof(uint32_t);
        do {
            m_Peer->m_Socket->Write(pack.m_Raw+lastCount, to_process);
            if (m_Peer->m_Socket->Error()) {
                m_pFrame->log(wxT("Socket Write error"));
	      delete msg;
                return true;
            }
            m_pFrame->log(wxString::Format("Wrote %d\n", m_Peer->m_Socket->LastCount()));
            lastCount += m_Peer->m_Socket->LastCount();
            to_process -= m_Peer->m_Socket->LastCount();
        } while(!m_Peer->m_Socket->Error() && to_process != 0);
    }
    delete msg;
    //Job j(Job::THREAD_READ_DATA_ID, NULL);
    //m_Peer->m_Queue->AddNewJob(j);

    return true;
}

bool PeerConnectionMonitor::ReadData() {
    int header;
    int lastCount = 0;
    
    if (m_Peer->m_Socket->IsConnected())
    {
        if (!m_Peer->m_Socket->WaitForRead(0,500)) {
            Job j(Job::THREAD_READ_DATA_ID, NULL);
            m_Peer->m_Queue->AddNewJob(j);

            return true;
        }
        int to_process = 4;
        do {
            m_Peer->m_Socket->Read((BYTE*)&header+lastCount,to_process);
            if (m_Peer->m_Socket->Error())
            {
                //m_pFrame->log(wxT("Socket Read error"));

	      wxCommandEvent* evt = new wxCommandEvent(wxEVT_PEER_DEPARTURE, wxID_ANY);
	      evt->SetString(m_Peer->m_Addr.IPAddress());
	      evt->SetInt(m_Peer->m_RepPort);
	      ((wxEvtHandler*)m_pFrame)->QueueEvent( evt );

                return true;
            }
            
            lastCount += m_Peer->m_Socket->LastCount();
            to_process -= m_Peer->m_Socket->LastCount();
        } while (!m_Peer->m_Socket->Error() && to_process != 0);

        to_process = header;
        lastCount = 0;
        BYTE* buffer = new BYTE[header];
        do {
            m_Peer->m_Socket->Read(buffer+lastCount,to_process);
            if (m_Peer->m_Socket->Error())
            {
                //m_pFrame->log(wxT("Socket Read error"));

	      wxCommandEvent* evt = new wxCommandEvent(wxEVT_PEER_DEPARTURE, wxID_ANY);
	      evt->SetString(m_Peer->m_Addr.IPAddress());
	      evt->SetInt(m_Peer->m_RepPort);
	      ((wxEvtHandler*)m_pFrame)->QueueEvent( evt );

                return true;
            }
            m_pFrame->log(wxString::Format("Read %d\n", m_Peer->m_Socket->LastCount()));
            lastCount += m_Peer->m_Socket->LastCount();
            to_process -= m_Peer->m_Socket->LastCount();
        } while (!m_Peer->m_Socket->Error() && to_process != 0);

        PRIME::PRIMEMessage* msg = new PRIME::PRIMEMessage;
        msg->ParseFromArray(buffer, header);
        delete buffer;
        PeerMessage* pm = new PeerMessage;

        pm->m_Msg = msg;
        pm->m_Peer = m_Peer;

        wxCommandEvent* evt = new wxCommandEvent(wxEVT_INC_PRIME_MESSAGE, wxID_ANY);
        evt->SetClientData((void*)pm);
        evt->SetString(m_Peer->m_Addr.IPAddress());

        ((wxEvtHandler*)m_pFrame)->QueueEvent( evt );
    }

    Job j(Job::THREAD_READ_DATA_ID, NULL);
    m_Peer->m_Queue->AddNewJob(j);

    return true;
}
