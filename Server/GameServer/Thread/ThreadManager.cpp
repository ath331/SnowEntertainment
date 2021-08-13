#include "ThreadManager.h"

void ThreadManager::Add( HANDLE threadHandle )
{
	_threadVec.push_back( threadHandle );
}
