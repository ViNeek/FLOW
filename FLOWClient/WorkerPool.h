#ifndef __WORKER_POOL_HEADER_H__
#define __WORKER_POOL_HEADER_H__

#include "../fwUtilities.h"
#include "Interface.h"
#include "JobQueue.h"
#include "JobHandler.h"

class MainFrame;

class WorkerPool
{
public:
    WorkerPool(	MainFrame* pFrame );
	~WorkerPool();
	void AssignJob(Job j);
private:
	//Parent frame
	MainFrame*					m_pFrame;

	//Pool State
	std::vector<JobQueue *> m_Workers;
	wxMutex					m_MutexPool; // protects pool access
};

#endif // __WORKER_POOL_HEADER_H__