#include "fwDecodingThread.h"

#include "fwJobQueue.h"
#include "fwInterface.h"
#include "fwInputHandler.h"
#include "fwFrameQueue.h"
#include "fwPacketQueue.h"

DecodingThread::DecodingThread(MainFrame* evtHandler, InputHandler* in_h, JobQueue* jqueue, PacketQueue* out_buffer, FrameQueue* fqueue) {
    m_pFrame = evtHandler;
    m_jQueue = jqueue;
    m_InHandle = in_h;
    m_OutBuffer = out_buffer;
    m_FrameQueue = fqueue;
}

DecodingThread::~DecodingThread(){

}

wxThread::ExitCode DecodingThread::Entry( ) {
	
	while( OnJob() );
	
	//Clean Up and inform Main thread
	//m_pFrame->log("Shit Happen");
	//printf("Shit Happen\n");
	return NULL;
}

bool DecodingThread::OnJob() {
    Job j = m_jQueue->Pop();
    switch(j.m_Type)
    {
    case Job::THREAD_OPEN_STREAM_ID: // thread should exit
        return OpenStream();
        break;
    case Job::THREAD_DELAY_DECODING_ID: // process a job that terminates with an error
    //ReadP2PSessionProps();
        return true;
        break;
    case Job::THREAD_NEXT_CHUNK_ID:
        NextChunk();
        return true;
        break;                                         
    case Job::THREAD_EXIT_ID:
        m_InHandle->Close();
        //m_OutHandle->Close();
        //delete m_FrameQueue;
        delete m_jQueue;
        delete m_OutBuffer;
        //delete m_InHandle;
        //delete m_OutHandle;
        return false;
        break;
    default:
        //Mo way!
        return false;
        break;
    } // switch(j.m_Type)

    return false;
}

void DecodingThread::NextChunk() {
    m_InHandle->NextChunk(m_FrameQueue);
}

bool DecodingThread::OpenStream() {
	wxCharBuffer cbuffer;
	bool res;
          res = m_InHandle->Open();
	if ( res == true ) {
	          wxCommandEvent* evt = new wxCommandEvent(wxEVT_INPUT_READY, wxID_ANY);
		evt->SetInt(1);
		evt->SetString(wxT("Input Ready"));
                    m_pFrame->log(wxT("inside!!"));
		wxQueueEvent( m_pFrame , evt );
	} else {
		wxCommandEvent* evt = new wxCommandEvent(wxEVT_INPUT_READY, wxID_ANY);
		evt->SetInt(0);
		evt->SetString(wxT("Input Ready"));

		wxQueueEvent( m_pFrame , evt );
	}
          
	return res;
}