#pragma once
#include <winsock2.h>
#include <vector>

class ThreadManager
{
public:
	void pushIoThread( HANDLE threadHandle );
	int GetThreadCount() { return (int)_ioThreadVec.size(); };

	std::vector< HANDLE >* GetThreadVecPtr() { return &_ioThreadVec; };

private:
	std::vector< HANDLE > _ioThreadVec;
	std::vector< HANDLE > _logicThreadVec;

};

