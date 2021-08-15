#pragma once
#include "../../GameServer/Client/ClientManager.h"

#include <WinSock2.h>

class AcceptManager
{
public:
	AcceptManager( SOCKET serverSock );
	~AcceptManager() {};

	void Accept();
	void ProcessForIOCP( HANDLE completionPort, ClientSocketDataPtr clientData );

private:
	SOCKET _serverSock;

};

