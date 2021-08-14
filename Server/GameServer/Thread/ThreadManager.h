#pragma once
#include <winsock2.h>
#include <vector>

class ThreadManager
{
public:
	void Add( HANDLE threadHandle );
	int GetThreadCount() { return (int)_threadVec.size(); };

	std::vector< HANDLE >* GetThreadVecPtr() { return &_threadVec; };

private:
	std::vector< HANDLE > _threadVec;

};

