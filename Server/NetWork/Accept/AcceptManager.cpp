#include "AcceptManager.h"
#include "../../Core/Log/Log.h"
#include "../../GameServer/OverlappedCustom.h"

#include <iostream>
#include <MSWSock.h>

#pragma comment(lib,"mswsock.lib")

AcceptManager::AcceptManager( SOCKET serverSock ) :
	_serverSock( serverSock )
{

}

void AcceptManager::Accept()
{
	OverlappedCustomPtr overlapped = std::make_shared<OverlappedCustom>();
	if ( !overlapped )
	{
		WARNING_LOG( "OverlappedCustomPtr make error" );
		return;
	}

	overlapped->iocpMode = EIocpMode::IOCP_ACCEPT;
	overlapped->clientSock = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );

	if ( AcceptEx( _serverSock, overlapped->clientSock, overlapped->buffer, 0,
		sizeof( SOCKADDR_IN ) + 16, sizeof( SOCKADDR_IN ) + 16,
		NULL, (LPOVERLAPPED)& overlapped ) 
		== false )
	{
		int errorCode = WSAGetLastError();
		if ( errorCode != WSA_IO_PENDING )
		{
			WARNING_LOG( "AcceptEx() error" );
			//std::cout << errorCode << std::endl;
			return;
		}
	}
}

void AcceptManager::ProcessForIOCP( HANDLE completionPort, ClientSocketDataPtr clientData )
{
	if ( !clientData )
		return;

	unsigned int recvBytes, flags = 0;

	SOCKET clientSock = clientData->hClntSock;
	int addrLen = sizeof( clientData->clntAdr );

	OverlappedCustomPtr overlapped;
	if ( !overlapped )
		return;

	std::cout << "[ Accept ] SOCKET is " << clientSock << std::endl;

	ClientSocketDataPtr _handleInfo;
	if ( !_handleInfo )
		return;

	_handleInfo->hClntSock = clientSock;
	memcpy( &( _handleInfo->clntAdr ), &clientData->clntAdr, addrLen );

	CreateIoCompletionPort( (HANDLE)clientSock, completionPort, ( unsigned long long )&_handleInfo, 0 );

	overlapped->wsaBuf.len = BUF_SIZE;
	overlapped->wsaBuf.buf = overlapped->buffer;
	overlapped->iocpMode = EIocpMode::IOCP_RECV;

	WSARecv( _handleInfo->hClntSock, &( overlapped->wsaBuf ),
		1, (LPDWORD)& recvBytes, (LPDWORD)& flags, &( overlapped->overlapped ), NULL );

	Accept();
}

