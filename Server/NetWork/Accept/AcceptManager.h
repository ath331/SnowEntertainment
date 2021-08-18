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
	void ProcessForIOCP( ClientSocketDataPtr clientData );

private:
	OverlappedCustom overlapped;
	SOCKET _serverSock;
	HANDLE _completionPort;

	char _buf[ 1024 ];
	DWORD _len = 10;
};

