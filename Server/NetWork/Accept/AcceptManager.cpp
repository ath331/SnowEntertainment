#include "AcceptManager.h"
#include "../../Core/LogCustom/Log.h"
#include "../../GameServer/OverlappedCustom.h"

#include <iostream>
#include <MSWSock.h>

#pragma comment(lib,"mswsock.lib")

AcceptManager::AcceptManager( SOCKET serverSock, HANDLE completionPort ) :
	_serverSock( serverSock ), _completionPort( completionPort )
{
	overlapped.Init( EIocpMode::IOCP_ACCEPT );

	memset(_buf, 0, sizeof(_buf));
	_len = 0;
}

void AcceptManager::Accept()
{
	overlapped.clientSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	if ( AcceptEx( _serverSock, overlapped.clientSock, _buf, 0,
		sizeof( SOCKADDR_IN ) + 16, sizeof( SOCKADDR_IN ) + 16,
		&_len, (LPOVERLAPPED)&overlapped )
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

void AcceptManager::ProcessForIOCP( SOCKET sock )
{
	unsigned int recvBytes, flags = 0;

	SOCKET clientSock = sock;

	std::cout << "[ Accept ] SOCKET is " << clientSock << std::endl;
	CreateIoCompletionPort((HANDLE)clientSock, _completionPort, clientSock, 0);

	OverlappedCustom* overlapped = new OverlappedCustom;
	if ( !overlapped )
		return;

	overlapped->wsaBuf.len = BUF_SIZE;
	overlapped->wsaBuf.buf = overlapped->buffer;
	overlapped->iocpMode = EIocpMode::IOCP_RECV;

	WSARecv( clientSock, &( overlapped->wsaBuf ),
		1, (LPDWORD)& recvBytes, (LPDWORD)& flags, &( overlapped->overlapped ), NULL );

	Accept();
}

