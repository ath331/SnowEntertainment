#include "AcceptManager.h"
#include "../../GameServer/OverlappedCustom.h"
#include <iostream>
#include <MSWSock.h>

#pragma comment(lib,"mswsock.lib")

void AcceptManager::Accept( SOCKET serverSock )
{
	OverlappedCustomPtr overlapped;
	if ( !overlapped )
	{
		std::cout << "OverlappedCustomPtr make error" << std::endl;
		return;
	}

	overlapped->clientSock = WSASocket( PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );

	//AcceptEx( serverSock, overlapped->clientSock, );
}
