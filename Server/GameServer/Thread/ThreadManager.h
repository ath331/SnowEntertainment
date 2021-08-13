#pragma once
#include <winsock2.h>
#include <vector>

class ThreadManager
{
public:
	void Add( HANDLE threadHandle );

private:
	std::vector< HANDLE > _threadVec;

};

