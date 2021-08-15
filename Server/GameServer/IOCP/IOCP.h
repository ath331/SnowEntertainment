#pragma once
#include "../Core/Parser/CConfigParser.h"
#include "../Enum.h"
#include <winsock2.h>

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

	static unsigned int WINAPI ProcessIocp( LPVOID iocpPtr );

private:
	std::string       _serverIP;
	std::string       _serverPORT;
	int               _iocpThreadCount;

	HANDLE            _completionPort;

	SOCKET            _servSock;

	BindManager*      _bindManager;
	ListenManager*    _listenManager;
	ThreadManager*    _ioThreadManager;
	AcceptManager*    _acceptManager;

private:
	void _CreateCompletionPort();
	void _ReadyConnect();
};

