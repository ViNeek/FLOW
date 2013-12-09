#include "JobHandler.h"

static void checkerror(int rtperr);

JobHandler::JobHandler(MainFrame* evtHandler, JobQueue* jqueue) : wxThread(wxTHREAD_DETACHED) {
	m_pFrame = evtHandler;
	m_jQueue = jqueue;
}

JobHandler::~JobHandler(){

}

wxThread::ExitCode JobHandler::Entry( ) {
	while( OnJob() );

	return NULL;
}

bool JobHandler::OnJob() {
	/*Job job = m_jQueue->Pop(); // pop a job from the queue. this will block the worker thread if queue is empty
	
	switch(job.m_Type)
    {
	case Job::THREAD_SERVER_MESSAGE_ID:
		//DecodeMessage((wxSocketBase*)job.m_Arg);
		return true;
		break;
	case Job::THREAD_INIT_CS_SESSION_ID:		// thread should exit
	//	InitCSSession((ServerSession*)job.m_Arg);
        return true;
		break;
	case Job::THREAD_WRITE_CHUNK:		// thread should exit
	//	WriteChunk((ProcessedContent*)job.m_Arg);
		return true;
		break;
    case Job::THREAD_INIT_RTP_SESSION_ID:		// process a job that terminates with an error
        //InitRTPSession((int*)job.m_Arg);
		return true;
        break;
    case Job::THREAD_INIT_P2P_SESSION_ID:
	//	InitP2PSession((ServerSession*)job.m_Arg);
		return true;
		break;
	case Job::THREAD_EXIT_ID:
	//	delete m_jQueue;
		return false;
		break;
    default:
		return true;
		break; // default
    } // switch(job.m_cmd)*/

    return true;
}

void JobHandler::WriteChunk( ProcessedContent* cont ) {
	/*wxMutexLocker lock(*cont->m_Guard);
	//m_pFrame->log("Printing");
	//jrtplib::RTPTime::Wait(jrtplib::RTPTime(0,33000));
	if ( cont->m_Content )
		cont->m_OutHnd->WriteChunk(*cont->m_Content);*/
}

void JobHandler::DecodeMessage(wxSocketBase* sock) {
	/*int to_process;
	int header;
	m_pFrame->log(wxT("Incoming!!!!"));
	sock->SetFlags(wxSOCKET_WAITALL);
	to_process = 4;
    do
    {
		sock->Read(&header,to_process);
        if (sock->Error())
        {
			return;
        }
        to_process -= sock->LastCount();
    } while (!sock->Error() && to_process != 0);

	BYTE* buffer = new BYTE[header];
	do
    {
		sock->Read(buffer,to_process);
        if (sock->Error())
        {
			return;
        }
		to_process -= sock->LastCount();
    } while (!sock->Error() && to_process != 0);

	PRIME::PRIMEMessage msg;
	msg.ParseFromArray(buffer, header);
	//m_pFrame->log(wxT("Incoming!!!!"));
	switch( msg.type() ) {
	case PRIME::PRIMEMessage_MsgType_PRIME_REPLY:
	{
		PRIME::Reply reply;
		reply = msg.reply();
		switch( reply.type() ) {
		case PRIME::Reply_ReplyType_CS_SESSION_INIT:
		{
			m_pFrame->log(wxT("Incoming Reply!!!!"));
			PRIME::Reply_CSSIReply csrep = reply.cs_session_state();
			int* RTPport = new int;
			*RTPport = csrep.streamport();
			Job j(Job::THREAD_INIT_RTP_SESSION_ID,RTPport);
			m_jQueue->AddNewJob(j);
			break;
		}
		case PRIME::Reply_ReplyType_P2P_SESSION_INIT:
		default:
			break;
		}
	}
		break;
	case PRIME::PRIMEMessage_MsgType_PRIME_REPORT:
		break;
	case PRIME::PRIMEMessage_MsgType_PRIME_REQUEST:
	{
		PRIME::Request req;
		req = msg.request();
		switch( req.type() ) {
		case PRIME::Request_RequestType_P2P_SESSION_INIT:
			break;
		case PRIME::Request_RequestType_CS_SESSION_INIT:
		default:
			break;
		}
	}
		break;
	default:
		break;
	}

	delete [] buffer;*/
}

bool JobHandler::InitRTPSession( int* RTPport ) {
	
	//m_pFrame->setRTPstate(*RTPport);

	return true;
}

bool JobHandler::InitP2PSession( ServerSession* sess ) {
	/*wxIPV4address serv_addr;
	serv_addr.Hostname(sess->m_SreamIpStr);
	serv_addr.Service(sess->m_StreamPort);
	wxSocketClient* sock = sess->m_Socket;
	sock->SetTimeout(1000);

	int to_process;
	
	if (!sock->Connect(serv_addr)) {
		 wxString s = wxString::Format(wxT("Cannot connect to %s:%d"),serv_addr.IPAddress().c_str(), serv_addr.Service());
		 m_pFrame->log(s);
		 // delete SocketClient;
		 return false;
    } else {
		if ( ! sock->IsConnected() )
			return false;

        m_pFrame->log(wxT("Connection Established! Sending P2P Request"));
		PRIME::Request req;
		PRIME::Request_P2PSIRequest stats;
		req.set_type(PRIME::Request_RequestType_P2P_SESSION_INIT);
		stats.set_dbw(sess->m_dbw);
		stats.set_ubw(sess->m_ubw);
		req.mutable_p2p_session_props()->CopyFrom(stats);
		PRIMEPacket pack(req);

		to_process = pack.m_Length + sizeof(int);
        do {
			sock->Write(pack.m_Raw,pack.m_Length + sizeof(int));
            if (sock->Error()) {
				m_pFrame->log(wxT("Socket Write error"));
                return false;
            }
            to_process -= sock->LastCount();
        } while(!sock->Error() && to_process != 0);
	}*/

    return true;
}

bool JobHandler::InitCSSession( ServerSession* sess ) {
	/*wxIPV4address serv_addr;
	serv_addr.Hostname(sess->m_SreamIpStr);
	serv_addr.Service(sess->m_StreamPort);
	wxSocketClient* sock = sess->m_Socket;
	sock->SetTimeout(1000);

	int to_process;
	
	if (!sock->Connect(serv_addr)) {
		 wxString s = wxString::Format(wxT("Cannot connect to %s:%d"),serv_addr.IPAddress().c_str(), serv_addr.Service());
		 m_pFrame->log(s);
		 // delete SocketClient;
		 return false;
    } else {
		if ( ! sock->IsConnected() )
			return false;

        m_pFrame->log(wxT("Connection Established! Sending CS Request"));
		PRIME::Request req;
		PRIME::Request_CSSIRequest stats;
		req.set_type(PRIME::Request_RequestType_CS_SESSION_INIT);
		stats.set_dbw(sess->m_dbw);
		req.mutable_cs_session_props()->CopyFrom(stats);
		PRIMEPacket pack(req);

		to_process = pack.m_Length + sizeof(int);
        do {
			sock->Write(pack.m_Raw,pack.m_Length + sizeof(int));
            if (sock->Error()) {
				m_pFrame->log(wxT("Socket Write error"));
                return false;
            }
            to_process -= sock->LastCount();
        } while(!sock->Error() && to_process != 0);
	}*/

    return true;
}

static void checkerror(int rtperr)
{
	//if (rtperr < 0)
	//{
	//	std::cerr << "ERROR: " << jrtplib::RTPGetErrorString(rtperr) << std::endl;
	//	exit(-1);
	//}
}
