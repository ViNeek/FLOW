#ifndef __DECODING_THREAD_HEADER_H__
#define __DECODING_THREAD_HEADER_H__

#include "../fwUtilities.h"

class MainFrame;
class InputHandler;
class JobQueue;
class FrameQueue;
class PacketQueue;

class DecodingThread : public wxThread
{
public:
	DecodingThread(MainFrame* evtHandler, InputHandler* in_h, JobQueue* jqueue, PacketQueue* out_buffer, FrameQueue* fqueue);
	~DecodingThread();

private:
	//parent frame
	MainFrame*				m_pFrame;

    //FrameQueue
    FrameQueue*              m_FrameQueue;
    
    //Packet Buffer
    PacketQueue*            m_OutBuffer;
    
	//Job Queue
	JobQueue*				m_jQueue;

	//Input Handle
	InputHandler*			m_InHandle;
	
	//Entry Point
    wxThread::ExitCode		Entry();

	//Job Looper
	bool					OnJob();
	
	//Job handlers
	bool OpenStream();
	void NextChunk();
    //void ReadP2PSessionProps();
};

#endif //__DECODING_THREAD_HEADER_H__