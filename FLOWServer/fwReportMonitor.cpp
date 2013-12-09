#include "fwReportMonitor.h"

#include "fwJob.h"
#include "fwJobQueue.h"
#include "fwInterface.h"
#include "fwPeerState.h"

ReportMonitor::ReportMonitor(MainFrame* evtHandler, JobQueue* jqueue, wxDatagramSocket* in_sock, wxDatagramSocket* out_sock) {
    m_jQueue = jqueue;
    m_OutReportSocket = out_sock;
    m_IncReportSocket = in_sock;
    m_pFrame = evtHandler;
}

wxThread::ExitCode ReportMonitor::Entry( ) {
	
    while( OnJob() );

    return NULL;
}

bool ReportMonitor::OnJob() {
    Job j = m_jQueue->Pop();
    switch(j.m_Type)
    {
    case Job::THREAD_READ_DATA_ID: // thread should exit
        return ReadData();
        break;
    case Job::THREAD_WRITE_DATA_ID: // process a job that terminates with an error
        return WriteData((PRIMEPacket*) j.m_Arg);
        break;
    case Job::THREAD_HOLE_PUNCH_ID: // process a job that terminates with an error
        return HolePunch((PRIMEPacket*) j.m_Arg);
        break;
    case Job::THREAD_EXIT_ID:
        //Friendly goodbye probably
        delete m_jQueue;
        m_OutReportSocket->Destroy();
        m_IncReportSocket->Destroy();
        printf("\nSuavemente\n");
        return false;
        break;
    default:
        //Mo way!
        return false;
        break;
    } // switch(j.m_Type)
    
    return false;
}

bool ReportMonitor::HolePunch(PRIMEPacket* pkt) {
    int to_process = -1;
    int lastCount = 0;
    if (m_IncReportSocket->IsOk())
    {
        for(int i = 0; i < 5; i++ ) {
	  lastCount = 0;
	  to_process = pkt->m_Length + sizeof(uint32_t);
	  //wxThread::Sleep(20);
	  do {
	      m_IncReportSocket->SendTo(pkt->m_Destination, pkt->m_Raw+lastCount,(pkt->m_Length + sizeof(int)) - lastCount);
	      if (m_IncReportSocket->Error())
	      {
		wxSocketError e = m_IncReportSocket->LastError();
		m_pFrame->log(wxString::Format("Write Error vre %d\n", e));
		//Job j(Job::THREAD_READ_DATA_ID, NULL);
		//m_jQueue->AddNewJob(j);

		delete pkt;

		return true;
	      }
	      lastCount += m_IncReportSocket->LastCount();
	      to_process -= m_IncReportSocket->LastCount();
	  } while(!m_IncReportSocket->Error() && to_process != 0);
        }
    }
    delete pkt;
    //m_pFrame->log(wxString::Format("Type %d\n", pkt->m_Length));

    //Job j(Job::THREAD_READ_DATA_ID, NULL);
    //m_jQueue->AddNewJob(j);

    return true;
}

bool ReportMonitor::WriteData(PRIMEPacket* pkt) {
    int to_process = -1;
    int lastCount = 0;
    if (m_IncReportSocket->IsOk())
    {
        to_process = pkt->m_Length + sizeof(uint32_t);
        do {
            m_IncReportSocket->SendTo(pkt->m_Destination, pkt->m_Raw+lastCount,(pkt->m_Length + sizeof(int)) - lastCount);
            if (m_IncReportSocket->Error())
            {
                wxSocketError e = m_IncReportSocket->LastError();
                m_pFrame->log(wxString::Format("Write Error vre %d\n", e));
	      //Job j(Job::THREAD_READ_DATA_ID, NULL);
	      //m_jQueue->AddNewJob(j);

	      delete pkt;

                return true;
            }
            lastCount += m_IncReportSocket->LastCount();
            to_process -= m_IncReportSocket->LastCount();
        } while(!m_IncReportSocket->Error() && to_process != 0);
    }
    delete pkt;
    //m_pFrame->log(wxString::Format("Type %d\n", pkt->m_Length));

    //Job j(Job::THREAD_READ_DATA_ID, NULL);
    //m_jQueue->AddNewJob(j);

    return true;
}

bool ReportMonitor::ReadData() {
    int header;
    BYTE b[4096];
    //m_pFrame->log(wxString::Format("Hurray %d", m_jQueue->QueueSize()));
    int lastCount = 0;
    if (m_IncReportSocket->IsOk())
    {
        if (!m_IncReportSocket->WaitForRead(0,200)) {
            Job j(Job::THREAD_READ_DATA_ID, NULL);
            m_jQueue->AddNewJob(j);
	  //m_pFrame->log(wxString::Format("Wait on Reading"));
            return true;
        }
        m_pFrame->log(wxString::Format("Reading"));
        wxIPV4address addr;
        int to_process = 4096;
        do {
            m_IncReportSocket->RecvFrom(addr, (BYTE*)&b[0]+lastCount,to_process-lastCount);
            if (m_IncReportSocket->Error())
            {
                wxSocketError e = m_IncReportSocket->LastError();
                //m_pFrame->log(wxString::Format("Error vre %d\n", e));
	      Job j(Job::THREAD_READ_DATA_ID, NULL);
	      m_jQueue->AddNewJob(j);
	      
                return true;
            }
            
            //printf("Last Count %d %d\n", m_IncReportSocket->LastCount(), m_IncReportSocket->Error());
            //printf("Last Error %d\n", m_IncReportSocket->LastError());
            if(lastCount == 0)
                break;
            lastCount += m_IncReportSocket->LastCount();
            to_process -= m_IncReportSocket->LastCount();
        } while (!m_IncReportSocket->Error() && to_process != 0);

        header = *((int*)&b[0]);
        //printf("Header %d\n", header);
        
        PRIME::PRIMEMessage* msg = new PRIME::PRIMEMessage;
        msg->ParseFromArray(&b[4], header);

        if( msg->type() == PRIME::PRIMEMessage_MsgType_PRIME_HOLE_PUNCH ) {
	  m_pFrame->log(wxString::Format("Punched from %s//%d\n", addr.IPAddress(), addr.Service()));

	  Job j(Job::THREAD_READ_DATA_ID, NULL);
	  m_jQueue->AddNewJob(j);

	  delete msg;

	  return true;
        }

        PeerMessage* pm = new PeerMessage;

        pm->m_Msg = msg;
        pm->m_Peer = NULL;

        //m_pFrame->log(wxString::Format("Type %d\n", msg->type()));
        wxCommandEvent* evt = new wxCommandEvent(wxEVT_INC_PRIME_MESSAGE, wxID_ANY);
        evt->SetClientData((void*)pm);
        evt->SetString(addr.IPAddress());

        ((wxEvtHandler*)m_pFrame)->QueueEvent( evt );
    }

    Job j(Job::THREAD_READ_DATA_ID, NULL);
    m_jQueue->AddNewJob(j);

    return true;
}