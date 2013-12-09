#ifndef __SERVER_CONNECTOR_HEADER_H__
#define __SERVER_CONNECTOR_HEADER_H__

#include "../fwUtilities.h"
#include "Interface.h"
#include "JobQueue.h"

class MainFrame;
class ServerConnector : public wxThread
{
public:
	ServerConnector(MainFrame *evtHandler, JobQueue *jq);
	~ServerConnector();

private:
	//parent frame
	MainFrame*				m_pFrame;

	//Job Queue
	JobQueue				*m_jQueue;

	//Socket
	wxSocketClient*			m_Socket;

    wxThread::ExitCode Entry();

	//Thread' s loop condition
	bool OnJob();

	//Start Server Session with specified values
	bool InitCSSession(ServerSession *sees);
};

#endif //__SERVER_CONNECTOR_HEADERR_H__