#pragma once
#include "../Core/Parser/CConfigParser.h"
#include "../Core/SingleTon/UxSingleTon.h"
#include <winsock2.h>

class IOCP :
	public CConfigParser
{
public:
	IOCP();
	~IOCP();

	void Run();

private:
	std::string _serverIP;
	std::string _serverPORT;

	SOCKET _servSock;
};

