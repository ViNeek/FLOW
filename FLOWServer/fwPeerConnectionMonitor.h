#ifndef __PEER_CONNECTION_MONITOR_HEADER_H__
#define __PEER_CONNECTION_MONITOR_HEADER_H__

#include "../fwUtilities.h"

class MainFrame;
class JobQueue;
class PeerState;

class PeerConnectionMonitor : public wxThread
{
public:
    PeerConnectionMonitor(MainFrame* evtHandler, PeerState* peer);
    ~PeerConnectionMonitor();

    MainFrame*			m_pFrame;
    PeerState*                          m_Peer;
private:
    wxThread::ExitCode		Entry();

    //Jobs
    bool                                ReadData();
    bool                                WriteData(PRIME::PRIMEMessage* msg);

    //Job Looper
    bool				OnJob();
};

#endif //__PEER_CONNECTION_MONITOR_HEADER_H__