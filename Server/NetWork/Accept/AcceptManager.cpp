#include "AcceptManager.h"
#include "../../Core/LogCustom/Log.h"
#include "../../GameServer/OverlappedCustom.h"

#include <iostream>
#include <MSWSock.h>

#pragma comment(lib,"mswsock.lib")

AcceptManager::AcceptManager( SOCKET serverSock, HANDLE completionPort ) :
	_serverSock( serverSock ), _completionPort( completionPort )
{

}

void AcceptManager::Accept()
{
	//overlapped = std::make_shared< OverlappedCustom >();
	overlapped.iocpMode = EIocpMode::IOCP_ACCEPT;
	/*if ( !overlapped )
	{
		WARNING_LOG( "OverlappedCustomPtr make error", WSAGetLastError() );
		return;
	}*/

	overlapped.iocpMode = EIocpMode::IOCP_ACCEPT;
	SOCKET c = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	overlapped.clientSock = c;
	
	ClientSocketData* _handleInfo = new ClientSocketData;
	if ( !_handleInfo )
		return;

	memset( &_handleInfo->clntAdr, 0, sizeof( _handleInfo->clntAdr ) );
	_handleInfo->hClntSock = overlapped.clientSock;

	CreateIoCompletionPort( (HANDLE)overlapped.clientSock, _completionPort, (unsigned long long) &_handleInfo, 0);
	int errorCode2 = WSAGetLastError();


	if ( AcceptEx( _serverSock, overlapped.clientSock, overlapped.buffer, 0,
		sizeof( SOCKADDR_IN ) + 16, sizeof( SOCKADDR_IN ) + 16,
		&overlapped.len, (LPOVERLAPPED)&overlapped )
		== false )
	{
		int errorCode = WSAGetLastError();
		if ( errorCode != WSA_IO_PENDING )
		{
			WARNING_LOG( "AcceptEx() error", WSAGetLastError() );
			return;
		}
	}
}

void AcceptManager::ProcessForIOCP( ClientSocketDataPtr clientData )
{
	if ( !clientData )
		return;

	unsigned int recvBytes, flags = 0;

	SOCKET clientSock = clientData->hClntSock;
	int addrLen = sizeof( clientData->clntAdr );

	std::cout << "[ Accept ] SOCKET is " << clientSock << std::endl;

	ClientSocketDataPtr _handleInfo;
	if ( !_handleInfo )
		return;

	OverlappedCustomPtr overlapped;
	if ( !overlapped )
		return;

	_handleInfo->hClntSock = clientSock;
	memcpy( &( _handleInfo->clntAdr ), &clientData->clntAdr, addrLen );

	overlapped->wsaBuf.len = BUF_SIZE;
	overlapped->wsaBuf.buf = overlapped->buffer;
	overlapped->iocpMode = EIocpMode::IOCP_RECV;

	WSARecv( _handleInfo->hClntSock, &( overlapped->wsaBuf ),
		1, (LPDWORD)& recvBytes, (LPDWORD)& flags, &( overlapped->overlapped ), NULL );

	Accept();
}

