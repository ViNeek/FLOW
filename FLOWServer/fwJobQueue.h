#ifndef __JOB_QUEUE_HEADERR_H__
#define __JOB_QUEUE_HEADERR_H__

#include "fwJob.h"

class JobQueue
{
public:

	JobQueue();
    void AddNewJob(const Job& job); // push a job onto the FIFO
    Job Pop();
    size_t QueueSize(); // Return number of pending jobs
  
private:
    uint64_t a;
	//Thread State
	std::queue<Job>	m_Jobs;
	wxMutex			m_MutexQueue; // protects queue access
	wxSemaphore		m_QueueCount; // semaphore count reflects number of queued jobs
};

#endif //__JOB_QUEUE_HEADERR_H__