#pragma once
#include "../Core/Parser/CConfigParser.h"
#include "../OverlappedCustom.h"
#include "../Enum.h"
#include <winsock2.h>

typedef struct    // socket info
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
} PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

class BindManager;
class ListenManager;
class ThreadManager;
class AcceptManager;
class IOCP :
	public CConfigParser
{
public:
	IOCP();
	~IOCP();

	void Run();
	void Wait();

private:
	std::string       _serverIP;
	std::string       _serverPORT;
	int               _iocpThreadCount;

	HANDLE            _completionPort;

	OverlappedCustomPtr _ioInfo;
	LPPER_HANDLE_DATA _handleInfo;

	SOCKET            _servSock;

	BindManager*      _bindManager;
	ListenManager*    _listenManager;
	ThreadManager*    _ioThreadManager;
	AcceptManager*    _acceptManager;

private:
	void _CreateCompletionPort();
	void _ReadyConnect();
};

