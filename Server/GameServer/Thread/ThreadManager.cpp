#include "ThreadManager.h"

void ThreadManager::pushIoThread( HANDLE threadHandle )
{
	_ioThreadVec.push_back( threadHandle );
}
