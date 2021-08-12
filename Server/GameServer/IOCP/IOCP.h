#pragma once
#include "../Core/Parser/CConfigParser.h"
#include "../Core/SingleTon/UxSingleTon.h"
#include <winsock2.h>

#define BUF_SIZE 100
#define READ	3
#define	WRITE	5

typedef struct    // socket info
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
} PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

typedef struct    // buffer info
{
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[ BUF_SIZE ];
	int rwMode;    // READ or WRITE
} PER_IO_DATA, * LPPER_IO_DATA;

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
	int _iocpThreadCount;

	WSADATA	_wsaData;
	HANDLE _completionPort;

	LPPER_IO_DATA _ioInfo;
	LPPER_HANDLE_DATA _handleInfo;

	SOCKET _servSock;
};

