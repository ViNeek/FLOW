#ifndef __RECEIVER_THREAD_HEADER_H__
#define __RECEIVER_THREAD_HEADER_H__

#include "../fwUtilities.h"

class MainFrame;
class JobQueue;
class PacketQueue;

class ReceiverThread : public wxThread
{
public:
    ReceiverThread(MainFrame* evtHandler, JobQueue* jqueue, PacketQueue* in_buffer, wxDatagramSocket* sock);
    ~ReceiverThread();

private:
    //parent frame
    MainFrame*				m_pFrame;

    //Packet Buffer
    PacketQueue*            m_InBuffer;
    
    //Job Queue
    JobQueue*				m_jQueue;

    //Datagram Socket
    wxDatagramSocket*                           m_Socket;
    //Entry Point
    wxThread::ExitCode		Entry();

    //Job Looper
    bool					OnJob();
    
    //Job handlers
    bool                            RecvChunk();	
    bool			  HolePunch(PRIMEPacket* pkt);
};

#endif //__RECEIVER_THREAD_HEADER_H__