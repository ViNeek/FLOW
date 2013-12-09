#ifndef __JOB_HANDLER_HEADER_H__
#define __JOB_HANDLER_HEADER_H__

#include "../fwUtilities.h"
#include "fwInterface.h"
#include "fwJobQueue.h"

class MainFrame;
class PeerState;
class ProcessedContent;

class JobHandler : public wxThread
{
public:
	JobHandler(MainFrame *evtHandler, JobQueue *jQueue);
	~JobHandler();
private:
	//Parent frame
	MainFrame*					m_pFrame;

	//Thread State
	JobQueue*					m_jQueue;
    int							m_ID;
  
    wxThread::ExitCode			Entry();
    
	//Job Loop
	bool						OnJob();

	//Job Handlers
	void						DecodeMessage( wxSocketBase* sock );
	bool						InitCSSession( ServerSession *sess );
	bool						InitRTPSession( int* RTPport );
	void						WriteChunk( ProcessedContent* cont );
	bool						InitP2PSession( ServerSession* sess );
};

#endif //__JOB_HANDLER_HEADER_H__