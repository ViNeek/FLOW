#include "fwJobQueue.h"

JobQueue::JobQueue() {}

void JobQueue::AddNewJob(const Job& job)	// push a job onto the FIFO
{
	wxMutexLocker lock(m_MutexQueue);	// lock the queue
	m_Jobs.push(job);					// push the new job
	m_QueueCount.Post();				// increment semaphore
}

Job JobQueue::Pop()
{
	Job element;
    m_QueueCount.Wait();		// wait for semaphore (=queue count to become positive)
    m_MutexQueue.Lock();		// lock queue
	element = m_Jobs.front();	// get next job
	m_Jobs.pop();				// Pop front
    m_MutexQueue.Unlock();		// unlock queue

	return element;				// return job entry
}

size_t JobQueue::QueueSize()			// Get Size
{
	wxMutexLocker lock(m_MutexQueue);	// lock queue until the size has been read
    
	return m_Jobs.size();
}

