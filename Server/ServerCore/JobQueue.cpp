#include "pch.h"
#include "JobQueue.h"
#include "GlobalQueue.h"

void JobQueue::Push( JobRef job, bool pushOnly )
{
	const int32 prevCount = _jobCount.fetch_add( 1 );
	_jobs.Push( job );

	if( prevCount == 0 )
	{
		if ( !LCurrentJobQueue && !pushOnly )
		{
			Execute();
		}
		else
		{
			// 여유가 되는 다른 쓰레드가 호출되도록 유도
			GGlobalQueue->Push( shared_from_this() );
		}
	}
}

void JobQueue::Execute()
{
	LCurrentJobQueue = this;

	while ( true )
	{
		Vector<JobRef> jobs;
		_jobs.PopAll( jobs );

		const int32 jobCount = static_cast<int32>( jobs.size() );
		for ( int32 i = 0 ; i < jobCount; i++ )
			jobs[ i ]->Execute();

		if ( _jobCount.fetch_sub( jobCount ) == jobCount )
		{
			LCurrentJobQueue = nullptr;
			return;
		}

		const uint64 now = ::GetTickCount64();
		if ( now >= LEndTickCount )
		{
			LCurrentJobQueue = nullptr;
			// 여유가 되는 다른 쓰레드가 호출되도록 유도
			GGlobalQueue->Push( shared_from_this() );
			break;
		}
	}
}
