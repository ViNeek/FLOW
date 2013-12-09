#ifndef __JOB_HEADERR_H__
#define __JOB_HEADERR_H__

#include "../fwUtilities.h"

class Job
{
public:
    enum JobType // list of commands that are currently implemented
    {
        THREAD_EXIT_ID=wxID_EXIT,			// thread exit code
        THREAD_REPORT_ID=wxID_HIGHEST+1,	          // thread should report state
        THREAD_NULL_ID,				// dummy job
        THREAD_PEER_REQUEST_ID,			// New Request from peer(Could be a new connection or an existing peer that wants input from the server)
        THREAD_INIT_CS_SESSION_ID,			// Init a Client-Server stream
        THREAD_INIT_P2P_SESSION_ID,			// Add new peer to the peer topology 
        THREAD_UPDATE_PEER_STATE_ID,		          // Peer requests new state(Probably after child or parent loss)
        THREAD_OPEN_FILE_ID,				// Open File
        THREAD_NEXT_CHUNK_ID,				// Next Chunk (There is always one in the queue)
        THREAD_SEND_CHUNK_ID,				// Send Chunk to peer
        THREAD_DECODE_NEXT_FRAME_ID,                        // Decode next frame for displaying
        THREAD_DELAY_DECODING_ID,			// Delay Decoding if buffer is full
        THREAD_WRITE_DATA_ID,
        THREAD_READ_DATA_ID,
        THREAD_CLOSE_CONNECTION_ID,
        THREAD_OPEN_STREAM_ID,
        THREAD_HOLE_PUNCH_ID,
        THREAD_RECV_CHUNK_ID,
        THREAD_SEND_CHUNK_WITH_POLICY_ID,
        THREAD_SLEEP_ID
    }; // enum Thread Jobs
  
    Job();
    Job(JobType jt, void *arg);

    JobType		m_Type;
	void		*m_Arg; //Must be a deep copy of the passing data, the thread takes full responsibility

	/*
	* For JobType == THREAD_CONTENT_ID m_Arg will probably be an AVPacket
	*
	*
	*
	*/
};

#endif //__JOB_HEADERR_H__