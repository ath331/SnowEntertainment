#pragma once
#include "../../GameServer/Client/ClientManager.h"
#include "../../GameServer/OverlappedCustom.h"

#include <WinSock2.h>

class OverlappedCustom;
class AcceptManager
{
public:
	AcceptManager( SOCKET serverSock, HANDLE completionPort );
	~AcceptManager() {};

	void Accept();
	void ProcessForIOCP( SOCKET clientSock );

private:
	OverlappedCustom _overlapped;
	SOCKET _serverSock;
	HANDLE _completionPort;

	char _buf[ BUF_SIZE ];
	DWORD _len = 0;
};

