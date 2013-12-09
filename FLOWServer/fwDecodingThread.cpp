#include "fwDecodingThread.h"

#include "fwJobQueue.h"
#include "fwInterface.h"
#include "fwPacketBuffer.h"
#include "fwInputHandler.h"
#include "fwOutputHandler.h"
#include "fwFrameQueue.h"
#include "fwPacketQueue.h"

DecodingThread::DecodingThread(MainFrame* evtHandler, wxString path, JobQueue* jqueue, PacketQueue* out_buffer, FrameQueue* fqueue) {
    m_pFrame = evtHandler;
    m_jQueue = jqueue;
    m_Url = path;
    m_OutBuffer = out_buffer;
    m_FrameQueue = fqueue;
}

DecodingThread::~DecodingThread(){

}

wxThread::ExitCode DecodingThread::Entry( ) {
	
	while( OnJob() );
	
	//Clean Up and inform Main thread
	
	return NULL;
}

bool DecodingThread::OnJob() {
	Job j = m_jQueue->Pop();
	switch(j.m_Type)
    {
	case Job::THREAD_OPEN_FILE_ID: // thread should exit
        return OpenFile();
		break;
	case Job::THREAD_DELAY_DECODING_ID: // process a job that terminates with an error
        //ReadP2PSessionProps();
		return true;
		break;
	case Job::THREAD_NEXT_CHUNK_ID: // dummy command
                //printf("MH\n");
              m_InHandle->NextChunk(m_FrameQueue, m_OutBuffer, m_OutHandle);
		return true;
		break;
	case Job::THREAD_EXIT_ID:
	    m_InHandle->Close();
              m_OutHandle->Close();
	    delete m_FrameQueue;
              delete m_jQueue;
              delete m_InHandle;
              delete m_OutHandle;
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
    //Chunk newChunk = m_InHandle->NextChunk();
}

bool DecodingThread::OpenFile() {
	wxCharBuffer cbuffer;
	m_InHandle = new InputHandler;
          m_OutHandle = new OutputHandler;
	cbuffer = m_Url.mbc_str();
	
          bool res = m_InHandle->Open(cbuffer.data(), m_OutHandle);
          
          if(res)
              m_OutHandle->Open(m_InHandle);
          
          m_pFrame->log(wxString::Format("FPS : %g", m_InHandle->m_FPS));
          m_FrameQueue->init(m_InHandle->m_FPS,m_InHandle->video_dec_ctx->pix_fmt,
              m_InHandle->video_dec_ctx->width, m_InHandle->video_dec_ctx->height);
          
        m_InHandle->InitStream(m_FrameQueue, m_OutBuffer, m_OutHandle);
    
	if ( res == true ) {
		wxCommandEvent* evt = new wxCommandEvent(wxEVT_INPUT_READY, wxID_ANY);
		evt->SetInt(1);
		evt->SetString(wxT("Input Ready"));
		m_pFrame->m_InHandle = m_InHandle;
                    m_pFrame->m_OutHandler = m_OutHandle;
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