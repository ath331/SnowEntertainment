#pragma once

#include <winsock2.h>
#include <iostream>

class NetWork
{
public:
	NetWork()
	{
		if ( WSAStartup( MAKEWORD( 2, 2 ), &_wsaData ) != 0 )
		{
			std::cout << "WSAStartup() error!" << std::endl;
			exit( 1 );
		}
	}

	~NetWork() {};

private:
	WSADATA _wsaData;
};