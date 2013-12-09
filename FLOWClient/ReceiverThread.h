#ifndef __RECEIVER_THREAD_HEADER_H__
#define __RECEIVER_THREAD_HEADER_H__

#include "../fwUtilities.h"
#include "JobQueue.h"
#include "Interface.h"
#include "ChunkBuffer.h"

class MainFrame;
class OutputHandler;

class ReceiverThread : public wxThread
{
public:
	ReceiverThread(MainFrame* evtHandler, JobQueue* jqueue, jrtplib::RTPUDPv4TransmissionParams transparams, jrtplib::RTPSessionParams sessparams);
	~ReceiverThread();

private:
	//parent frame
	MainFrame*								m_pFrame;

	//Job Queue
	JobQueue*								m_jQueue;

	//Chunk Buffer
	ChunkBuffer*							m_CBuffer;

	//Output Handle
	OutputHandler*							m_OutHandle;
	
	//Session
	jrtplib::RTPSession*					m_Sess;
	jrtplib::RTPUDPv4TransmissionParams		m_Transparams;
	jrtplib::RTPSessionParams				m_Sessparams;

	//Entry Point
    wxThread::ExitCode		Entry();

	//Session Guard
	wxMutex					m_SessionGuard;

	//Job Looper
	bool					OnJob();
	
	//Job handlers
	bool					Open();
	void					WriteChunk(Chunk* c);
	bool					ReadNextChunk();
    //void ReadP2PSessionProps();
};

#endif // __RECEIVER_THREAD_HEADER_H__