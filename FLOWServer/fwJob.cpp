#include "fwJob.h"

Job::Job() : m_Type(THREAD_NULL_ID), m_Arg(NULL) {}
Job::Job(JobType cmd, void *arg) : m_Type(cmd), m_Arg(arg) {}