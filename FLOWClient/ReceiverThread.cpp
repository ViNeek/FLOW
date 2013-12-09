#include "ReceiverThread.h"

ReceiverThread::ReceiverThread(MainFrame* evtHandler, JobQueue* jqueue, jrtplib::RTPUDPv4TransmissionParams transparams, jrtplib::RTPSessionParams sessparams) {
	m_pFrame = evtHandler;
	m_jQueue = jqueue;
	m_Transparams = transparams;
	m_Sessparams = sessparams;
	m_Sess = NULL;
	m_CBuffer = NULL;
}

ReceiverThread::~ReceiverThread(){

}

wxThread::ExitCode ReceiverThread::Entry( ) {
	
	while( OnJob() );
	
	//Clean Up and inform Main thread

	return NULL;
}

bool ReceiverThread::OnJob() {
	Job j = m_jQueue->Pop();
	switch(j.m_Type)
    {
	case Job::THREAD_SERVER_MESSAGE_ID: // thread should exit
		//return Open();
		break;
	case Job::THREAD_OPEN_OUTPUT_ID: // thread should exit
		return Open();
		break;
	case Job::THREAD_NEXT_CHUNK_ID: // c
        ReadNextChunk();
		return true;
		break;
	case Job::THREAD_EXIT_ID:
		return false;
		break;
    default:
		// Mo way!
		return false;
		break;
    } // switch(j.m_Type)

	return false;
}

void ReceiverThread::WriteChunk(Chunk* c) {
	//Chunk newChunk = m_InHandle->NextChunk();
	//m_CBuffer->PushChunk(&newChunk);
	//Chunk Buffer owns the data now
	//newChunk.Clear();
}

static wxMutex session_guard;

bool ReceiverThread::ReadNextChunk() {
	//wxMutexLocker lock(m_SessionGuard);
	//while(1) {
	m_Sess->BeginDataAccess();
	//m_pFrame->log("Got Packet");
	if (m_Sess->GotoFirstSourceWithData()) {
		//m_pFrame->log("Got Packet 2");
		do
		{
			jrtplib::RTPPacket *pack;
				
			while ((pack = m_Sess->GetNextPacket()) != NULL)
			{
				Chunk *cha = new Chunk;
				session_guard.Lock();
				cha->Deserialize((uint8_t *)pack->GetPayloadData());
				session_guard.Unlock();
				//wxString s;
				//s.Printf("Receiving %d", cha->id);
				//m_pFrame->log(s);
				if ( !m_CBuffer->PushChunk(cha) ) {
					wxCommandEvent* event = new wxCommandEvent(wxEVT_NEW_OUTPUT, wxID_ANY);
					event->SetString(wxString("New Content"));

					wxQueueEvent( m_pFrame, event );

					m_Sess->DeletePacket(pack);
					cha->Clear();
					delete cha;

					m_Sess->EndDataAccess();

#ifndef RTP_SUPPORT_THREAD
					int rtperr = m_Sess->Poll();
					if (rtperr < 0)
					{
						wxString s;
						s.Printf("ERROR: %s", jrtplib::RTPGetErrorString(rtperr).c_str());
						m_pFrame->log(s);
					}
#endif // RTP_SUPPORT_THREAD

					return true;
				}

				m_Sess->DeletePacket(pack);
				cha->Clear();
				delete cha;
			}
		} while (m_Sess->GotoNextSourceWithData());
	}
	
	m_Sess->EndDataAccess();

#ifndef RTP_SUPPORT_THREAD
	int rtperr = m_Sess->Poll();
	if (rtperr < 0)
	{
		wxString s;
		s.Printf("ERROR: %s", jrtplib::RTPGetErrorString(rtperr).c_str());
		m_pFrame->log(s);
	}
#endif // RTP_SUPPORT_THREAD
	//}
	//jrtplib::RTPTime::Wait(jrtplib::RTPTime(3));
	Job j(Job::THREAD_NEXT_CHUNK_ID, (void*)NULL); 
	m_jQueue->AddNewJob(j);
	
	return true;
}

bool ReceiverThread::Open() {
	wxCharBuffer cbuffer;
	m_OutHandle = new OutputHandler;
	m_Sess = new jrtplib::RTPSession;

	bool res = m_OutHandle->Open("test");

	if ( res == true ) {
		wxCommandEvent* evt = new wxCommandEvent(wxEVT_OUTPUT_READY, wxID_ANY);
		evt->SetInt(1);
		evt->SetString(wxT("Output Ready"));
		m_pFrame->setOutputHandle(m_OutHandle);
		m_CBuffer = new ChunkBuffer;
		m_pFrame->setBuffer(m_CBuffer);
		m_pFrame->log("Output Opened");
		wxQueueEvent( m_pFrame , evt );
	} else {
		wxCommandEvent* evt = new wxCommandEvent(wxEVT_OUTPUT_READY, wxID_ANY);
		evt->SetInt(0);
		evt->SetString(wxT("Output could not be opened"));
		m_pFrame->log("Output could not be opened");
		wxQueueEvent( m_pFrame , evt );
	}
	
	int rtperr = m_Sess->Create(m_Sessparams,&m_Transparams);
	
	if (rtperr < 0)
	{
		wxString s;
		s.Printf("ERROR: %s", jrtplib::RTPGetErrorString(rtperr).c_str());
		m_pFrame->log(s);
	} else {
		wxString s;
		s.Printf("RTP Session Created");
		m_pFrame->log(s);
	}

	return res;
}