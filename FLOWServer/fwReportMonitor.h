#ifndef __REPORT_MONITOR_HEADER_H__
#define __REPORT_MONITOR_HEADER_H__

#include "../fwUtilities.h"

class MainFrame;
class JobQueue;
class PacketQueue;

class ReportMonitor : public wxThread
{
public:
    ReportMonitor(MainFrame* evtHandler, JobQueue* jqueue, wxDatagramSocket* in_sock, wxDatagramSocket* out_sock);

private:
    //parent frame
    MainFrame*				m_pFrame;

    //Sockets
    wxDatagramSocket*                             m_IncReportSocket;
    wxDatagramSocket*                             m_OutReportSocket;

    //Job Queue
    JobQueue*				m_jQueue;

    //Entry Point
    wxThread::ExitCode		Entry();

    //Job Looper
    bool					OnJob();
    
    //Job handlers
    bool                                ReadData();
    bool                                WriteData(PRIMEPacket* pkt);
    bool				HolePunch(PRIMEPacket* pkt);
};

#endif //__REPORT_MONITOR_HEADER_H__