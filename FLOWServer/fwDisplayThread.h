//
//  fwDisplayThread.h
//  FLOWServer
//
//  Created by Nick Vitsas on 9/16/13.
//  Copyright (c) 2013 Nick Vitsas. All rights reserved.
//

#ifndef __DISPLAY_THREAD_HEADER_H__
#define __DISPLAY_THREAD_HEADER_H__

#include "../fwUtilities.h"

class MainFrame;
class JobQueue;

class DisplayThread : public wxThread
{
public:
	DisplayThread(MainFrame* evtHandler, JobQueue* jqueue);
	DisplayThread();
    
private:
	//parent frame
	MainFrame*				m_pFrame;
    
	//Job Queue
	JobQueue*				m_jQueue;
    
	//Entry Point
    wxThread::ExitCode		Entry();
    
	//Job Looper
	bool					OnJob();
};

#endif //__DISPLAY_THREAD_HEADER_H__
