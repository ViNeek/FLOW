//
//  fwDisplayThread.cpp
//  FLOWServer
//
//  Created by Nick Vitsas on 9/16/13.
//  Copyright (c) 2013 Nick Vitsas. All rights reserved.
//

#include "fwServerConnectionMonitor.h"
#include "fwJobQueue.h"
#include "fwInterface.h"

ServerConnectionMonitor::ServerConnectionMonitor(wxSocketBase* pSocket, MainFrame* evtHandler, JobQueue* jqueue) {
    m_pFrame = evtHandler;
    m_Queue = jqueue;
    m_Socket = (wxSocketClient*)pSocket;
    m_Socket->Notify(false);
    m_Socket->SetFlags(wxSOCKET_NOWAIT);
    //m_Socket->SetFlags(wxSOCKET_WAITALL);
}

ServerConnectionMonitor::~ServerConnectionMonitor() {

}

wxThread::ExitCode ServerConnectionMonitor::Entry( ) {
	
    while( OnJob() );

    return NULL;
}

bool ServerConnectionMonitor::OnJob() {
    Job j = m_Queue->Pop();
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
        delete m_Queue;
        m_Socket->Destroy();
        return false;
        break;
    default:
        //Mo way!
        return false;
        break;
    } // switch(j.m_Type)
    
    return false;
}

bool ServerConnectionMonitor::WriteData(PRIME::PRIMEMessage* msg) {
    if (!m_Socket->IsConnected())
    {
        wxIPV4address serv_addr;
        serv_addr.Hostname(m_pFrame->m_StreamAddress);
        serv_addr.Service(m_pFrame->m_StreamPort);

        m_Socket->Connect(serv_addr, false);

        int tries = 0;
        while ( !m_Socket->WaitOnConnect(0, 5000) && (tries < 3) )
        {
	  printf("Connecting ...\n");
            tries++;
        }

        if (!m_Socket->IsConnected())
            return false;

        m_Socket->GetPeer(m_Peer);
    }
    int to_process = -1;
    int lastCount = 0;
    if (m_Socket->IsConnected())
    {
        PRIMEPacket pack(*msg);
        to_process = pack.m_Length + sizeof(uint32_t);
        do {
            m_Socket->Write(pack.m_Raw+lastCount,(pack.m_Length + sizeof(int)) - lastCount);
            
            //m_pFrame->log(wxString::Format("Wrote %d\n", m_Socket->LastCount()));
            lastCount += m_Socket->LastCount();
            to_process -= m_Socket->LastCount();
        } while(!m_Socket->Error() && to_process != 0);
    }
    //m_pFrame->log(wxString::Format("Type %d\n", msg->type()));

    //Job j(Job::THREAD_READ_DATA_ID, NULL);
    //m_Queue->AddNewJob(j);

    return true;
}

bool ServerConnectionMonitor::ReadData() {
    int header;
    int lastCount = 0;
    //m_pFrame->log(wxT("Hurray"));
    if (m_Socket->IsConnected())
    {
        if (!m_Socket->WaitForRead(0,500)) {
            Job j(Job::THREAD_READ_DATA_ID, NULL);
            m_Queue->AddNewJob(j);

            return true;
        }
        int to_process = 4;
        do {
            m_Socket->Read((BYTE*)&header+lastCount,to_process);
            if (m_Socket->Error())
            {
                wxSocketError e = m_Socket->LastError();
                m_pFrame->log(wxString::Format("Error %d\n", e));
                return true;
            }
            
            lastCount += m_Socket->LastCount();
            to_process -= m_Socket->LastCount();
        } while (!m_Socket->Error() && to_process != 0);

        to_process = header;
        lastCount = 0;
        BYTE* buffer = new BYTE[header];
        do {
            m_Socket->Read(buffer+lastCount,to_process);
            if (m_Socket->Error())
            {
                wxSocketError e = m_Socket->LastError();
                m_pFrame->log(wxString::Format("Error 2 %d\n", e));
                return true;
            }
            m_pFrame->log(wxString::Format("Read %d\n", m_Socket->LastCount()));
            lastCount += m_Socket->LastCount();
            to_process -= m_Socket->LastCount();
        } while (!m_Socket->Error() && to_process != 0);
        
        PRIME::PRIMEMessage* msg = new PRIME::PRIMEMessage;
        msg->ParseFromArray(buffer, header);

        wxCommandEvent* evt = new wxCommandEvent(wxEVT_INC_PRIME_MESSAGE, wxID_ANY);
        evt->SetClientData((void*)msg);
        //evt->SetString(m_Addr.IPAddress());

        ((wxEvtHandler*)m_pFrame)->QueueEvent( evt );
    }

    Job j(Job::THREAD_READ_DATA_ID, NULL);
    m_Queue->AddNewJob(j);

    return true;
}
