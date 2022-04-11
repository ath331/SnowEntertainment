#pragma once
#include "JobQueue.h"

class GlobalQueue
{
public:
	GlobalQueue();
	~GlobalQueue();

	void        Push( JobQueueRef jobQueue );
	JobQueueRef Pop();

private:
	LockQueue<JobQueueRef> _jobQueues;

};

