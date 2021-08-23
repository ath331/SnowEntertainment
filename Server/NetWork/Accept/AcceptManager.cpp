#include "AcceptManager.h"
#include "../../Core/LogCustom/Log.h"
#include "../../GameServer/OverlappedCustom.h"
#include "..//Session/TcpSession.h"

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
	overlapped.Init( EIocpMode::IOCP_ACCEPT );
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

void AcceptManager::ProcessForIOCP( SOCKET clientSock )
{
	std::cout << "[ Accept ] SOCKET is " << clientSock << std::endl;

	TcpSession* session = new TcpSession( clientSock ); //TODO : clientSession을 따로 저장해 놔야할듯?
	CreateIoCompletionPort((HANDLE)clientSock, _completionPort, (ULONG_PTR)session, 0);

	session->_PostRecv();

	Accept();
}

