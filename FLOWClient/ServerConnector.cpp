#include "ServerConnector.h"

#if 0
ServerConnector::ServerConnector(MainFrame *evtHandler, JobQueue *jq) : wxThread(wxTHREAD_DETACHED), 
	m_pFrame(evtHandler),
	m_jQueue(jq)
{
	m_Socket = new wxSocketClient(wxSOCKET_BLOCK|wxSOCKET_WAITALL);
}

ServerConnector::~ServerConnector() {

}

wxThread::ExitCode ServerConnector::Entry( ) {
	// Start Thread Loop
	while( OnJob() );

	return NULL;
}

bool ServerConnector::InitCSSession(ServerSession *sess) {
	wxIPV4address serv_addr;
	serv_addr.Hostname(sess->m_SreamIpStr);
	serv_addr.Service(sess->m_StreamPort);
    m_Socket->SetTimeout(100);
	int to_process;

	if (!m_Socket->Connect(serv_addr)) {
		 wxString s = wxString::Format(wxT("Cannot connect to %s:%d"),serv_addr.IPAddress().c_str(), serv_addr.Service());
		 m_pFrame->log(s);
    } else {
        m_pFrame->log(wxT("Connection Established! Sending Request"));
		PRIME::Request req;
		PRIME::Request_CSSIRequest stats;
		req.set_type(PRIME::Request_RequestType_CS_SESSION_INIT);
		stats.set_dbw(sess->m_dbw);
		req.mutable_cs_session_props()->CopyFrom(stats);
		PRIMEPacket pack(req);

		to_process = pack.m_Length + sizeof(int);
        do {
			m_Socket->Write(pack.m_Raw,pack.m_Length + sizeof(int));
            if (m_Socket->Error()) {
				m_pFrame->log(wxT("Socket Write error"));
                return false;
            }
            to_process -= m_Socket->LastCount();
            //wxLogDebug(wxT("EventWorker: written %d bytes, %d bytes to do"),m_clientSocket->LastCount(),to_process);
        } while(!m_Socket->Error() && to_process != 0);
		/*
        if (!failed) {
            etype = WorkerEvent::RECEIVING;
            WorkerEvent e(this,etype);
            wxGetApp().AddPendingEvent(e);
            to_process = m_insize;
            do {
                m_clientSocket->Read(m_inbuf,m_insize);
                if (m_clientSocket->Error()) {
                    wxLogError(wxT("ThreadWorker: Read error"));
                    failed = true;
                    break;
                }
                to_process -= m_clientSocket->LastCount();
                //wxLogDebug(wxT("EventWorker: readed %d bytes, %d bytes to do"),m_clientSocket->LastCount(),to_process);
            } while(!m_clientSocket->Error() && to_process != 0);
        }

        char* outdat = (char*)m_outbuf+2;
        if (!failed && (memcmp(m_inbuf,outdat,m_insize) != 0))
        {
            wxLogError(wxT("Data mismatch"));
            failed = true;
        }*/
    }
}

bool ServerConnector::OnJob() {
	Job job = m_jQueue->Pop(); // pop a job from the queue. this will block the worker thread if queue is empty
	
	switch(job.m_Type)
    {
	case Job::THREAD_INIT_CSS_ID: // thread should exit
		InitCSSession((ServerSession *)job.m_Arg);
        return true;
	case Job::THREAD_INIT_P2PS_ID: // process a standard job
		//ReadCSSessionProps();
		return true;
		break;
	case Job::THREAD_NEXT_LOOP_ID: // process a job that terminates with an error
        //ReadP2PSessionProps();
		return true;
        break;
	case Job::THREAD_EXIT_ID:
		return false;
		break;
    default:
		return true;
		break; // default
    } // switch(job.m_Type)

	return true;
}

#endif