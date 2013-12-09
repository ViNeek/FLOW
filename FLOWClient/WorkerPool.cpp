#include "WorkerPool.h"

WorkerPool::WorkerPool(MainFrame* pFrame) {
	int CPUCount;

	m_pFrame = pFrame;
	CPUCount = wxThread::GetCPUCount();
	if ( CPUCount < 0 )
		CPUCount = 4;

	for ( int i = 0; i < CPUCount; i++ ) {
		JobQueue* jQueue = new JobQueue;
		JobHandler*	jHandler = new JobHandler(m_pFrame, jQueue);
		
		m_Workers.push_back(jQueue);
		if (jHandler->Create() == wxTHREAD_NO_ERROR) {
            jHandler->Run();
        }
        else
        {
			m_pFrame->log("Unable to create ThreadPool");
			break;
        }
	}
	m_pFrame->log("ThreadPool Created");
}

WorkerPool::~WorkerPool() {
	Job j;
	j.m_Type = Job::THREAD_EXIT_ID;
	for ( size_t i = 0; i < m_Workers.size(); i++ ) {
		m_Workers.at(i)->AddNewJob(j);
		//delete m_Workers.at(i);
	}
}

void WorkerPool::AssignJob(Job j) {
	int minCount = 1000000000L;
	int minPos = 0;

	wxMutexLocker lock(m_MutexPool);	// lock pool

	for ( size_t i = 0; i < m_Workers.size(); i++ ) {
		int queuePop = m_Workers.at(i)->QueueSize();
		if ( queuePop == 0 ) {
			m_Workers.at(i)->AddNewJob(j);
			return;
		} else if ( queuePop < minCount ) {
			minCount = queuePop;
			minPos = i;
		}
	}

	m_Workers.at(minPos)->AddNewJob(j);
}