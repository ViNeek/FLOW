#ifndef __SERVER_CONNECTION_MONITOR_HEADER_H__
#define __SERVER_CONNECTION_MONITOR_HEADER_H__

#include "../fwUtilities.h"

class MainFrame;
class JobQueue;

class ServerConnectionMonitor : public wxThread
{
public:
    ServerConnectionMonitor(wxSocketBase* pSocket, MainFrame* evtHandler, JobQueue* jqueue);
    ~ServerConnectionMonitor();

private:
    //parent frame
    MainFrame*			m_pFrame;
    JobQueue*                           m_Queue;

    wxSocketClient*                     m_Socket;
    wxIPV4address                       m_Peer;

    wxThread::ExitCode		Entry();
    bool                                ReadData();
    bool                                WriteData(PRIME::PRIMEMessage* msg);

    //Job Looper
    bool				OnJob();
};

#endif //__SERVER_CONNECTION_MONITOR_HEADER_H__