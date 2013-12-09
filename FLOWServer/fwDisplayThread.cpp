//
//  fwDisplayThread.cpp
//  FLOWServer
//
//  Created by Nick Vitsas on 9/16/13.
//  Copyright (c) 2013 Nick Vitsas. All rights reserved.
//

#include "fwDisplayThread.h"

#include "fwDecodingThread.h"

#include "fwJobQueue.h"
#include "fwInterface.h"
#include "fwInputHandler.h"

DisplayThread::DisplayThread(MainFrame* evtHandler, JobQueue* jqueue) {
	m_pFrame = evtHandler;
	m_jQueue = jqueue;
}

DisplayThread::DisplayThread(){
    
}

wxThread::ExitCode DisplayThread::Entry( ) {
	
	while( OnJob() );
	
	//Clean Up and inform Main thread
	m_pFrame->log("Shit Happen");
	return NULL;
}

bool DisplayThread::OnJob() {
	Job j = m_jQueue->Pop();
	switch(j.m_Type)
    {
        case Job::THREAD_DECODE_NEXT_FRAME_ID: // thread should exit
            return true;
            break;
        case Job::THREAD_DELAY_DECODING_ID: // process a job that terminates with an error
            //ReadP2PSessionProps();
            return true;
            break;
        case Job::THREAD_NEXT_CHUNK_ID: // dummy command
            return true;
            break;
        case Job::THREAD_EXIT_ID:
            return false;
            break;
        default:
            //Mo way!
            return false;
            break;
    } // switch(j.m_Type)
    
	return false;
}
