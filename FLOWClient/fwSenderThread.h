#ifndef __SENDER_THREAD_HEADER_H__
#define __SENDER_THREAD_HEADER_H__

#include "../fwUtilities.h"

class MainFrame;
class JobQueue;
class PacketQueue;

class Policy {
public:
    Policy(int32_t peer_index, int32_t max) : m_PeerIndex(peer_index), m_MaxPacketCount(max) {}
    Policy() : m_PeerIndex(0), m_MaxPacketCount(30) {}
    int32_t m_PeerIndex;
    int32_t m_MaxPacketCount;
};

class SenderThread : public wxThread
{
public:
    SenderThread(MainFrame* evtHandler, JobQueue* jqueue, PacketQueue* out_buffer, int session_type, wxDatagramSocket* sock);
    ~SenderThread();

private:
    //parent frame
    MainFrame*				m_pFrame;

    //Packet Buffer
    PacketQueue*            m_OutBuffer;
    
    //Job Queue
    JobQueue*				m_jQueue;

    int m_SessionType;

    //Datagram Socket
    wxDatagramSocket*                           m_Socket;
    //Entry Point
    wxThread::ExitCode		Entry();

    //Job Looper
    bool					OnJob();
    
    //Job handlers
    bool                            SendChunk();	
    bool                            SendChunkWithPolicy(Policy* pol);	
};

#endif //__SENDER_THREAD_HEADER_H__