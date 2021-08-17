#pragma once
#include "../../GameServer/Client/ClientManager.h"

#include <WinSock2.h>

class OverlappedCustom;
class AcceptManager
{
public:
	AcceptManager( SOCKET serverSock );
	~AcceptManager() {};

	void Accept();
	void ProcessForIOCP( HANDLE completionPort, ClientSocketDataPtr clientData );

private:
	OverlappedCustom* overlapped;
	SOCKET _serverSock;
	char _buf[ 1024 ];
	DWORD _len = 10;
};

