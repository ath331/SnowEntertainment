#include "IOCP.h"
#include <iostream>
#include <stdlib.h>
#include <process.h>
#include <windows.h>

#include "../NetWork/Accept/AcceptManager.h"
#include "../NetWork/Bind/BindManager.h"
#include "../NetWork/Listen/ListenManager.h"

#include "../Thread/ThreadManager.h"

using namespace std;

unsigned int WINAPI EchoThreadMain( LPVOID CompletionPortIO );

IOCP::IOCP() :
	CConfigParser("../Ini/GameServer.ini")
{
	_serverIP        = GetString( "SERVER_IP"         );
	_serverPORT      = GetString( "SERVER_PORT"       );
	_iocpThreadCount = GetInt   ( "IOCP_THREAD_COUNT" );

	cout << "[ IOCP Setting ]" << endl;
	cout << "IP   : "          << _serverIP        << endl;
	cout << "PORT : "          << _serverPORT      << endl;
	cout << "IoThreadCount : " << _iocpThreadCount << endl;

	_CreateCompletionPort();

	_ioThreadManager = new ThreadManager;
	if ( !_ioThreadManager )
	{
		cout << "new _ioThreadManager error" << endl;
	}

	for ( unsigned short i = 0; i < _iocpThreadCount; i++ )
	{
		_ioThreadManager->Add( (HANDLE)_beginthreadex( NULL, 0, EchoThreadMain, (LPVOID)_completionPort, 0, NULL ) );
	}

	_servSock = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );

	_ReadyConnect();
}

IOCP::~IOCP()
{
	if ( _bindManager )
		delete _bindManager;

	if ( _listenManager )
		delete _listenManager;

	if ( _ioThreadManager )
		delete _ioThreadManager;
}

void IOCP::_CreateCompletionPort()
{
	_completionPort = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );
}

void IOCP::_ReadyConnect()
{
	_bindManager = new BindManager( _serverPORT );
	if ( !_bindManager )
	{
		cout << "new Bind error" << endl;
		exit( 1 );
	}
	_bindManager->Bind( _servSock );

	_listenManager = new ListenManager;
	if ( !_listenManager )
	{
		cout << "new Listen error" << endl;
		exit( 1 );
	}
	_listenManager->Listen( _servSock );
}


void IOCP::Run()
{
	cout << endl <<"[ ----------SERVER RUN---------- ]" << endl;

	unsigned int recvBytes, flags = 0;

	while ( 1 )
	{
		SOCKET hClntSock;
		SOCKADDR_IN clntAdr;
		int addrLen = sizeof( clntAdr );

		hClntSock = accept( _servSock, (SOCKADDR*)& clntAdr, &addrLen );

		cout << "[ Accept ] SOCKET is " << hClntSock << endl;

		_handleInfo = new PER_HANDLE_DATA;
		if ( !_handleInfo )
			continue;

		_handleInfo->hClntSock = hClntSock;
		memcpy( &( _handleInfo->clntAdr ), &clntAdr, addrLen );

		CreateIoCompletionPort( (HANDLE)hClntSock, _completionPort, ( unsigned long long )_handleInfo, 0 );

		_ioInfo = new PER_IO_DATA;
		if ( !_ioInfo )
			continue;

		memset( &( _ioInfo->overlapped ), 0, sizeof( OVERLAPPED ) );
		_ioInfo->wsaBuf.len = BUF_SIZE;
		_ioInfo->wsaBuf.buf = _ioInfo->buffer;
		_ioInfo->rwMode = READ;

		WSARecv( _handleInfo->hClntSock, &( _ioInfo->wsaBuf ),
			1, (LPDWORD)& recvBytes, (LPDWORD)& flags, &( _ioInfo->overlapped ), NULL );
	}
}

unsigned int WINAPI EchoThreadMain( LPVOID pComPort )
{
	HANDLE completionPort = (HANDLE)pComPort;
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_HANDLE_DATA handleInfo = nullptr;
	LPPER_IO_DATA ioInfo = nullptr;
	DWORD flags = 0;

	while ( 1 )
	{
		GetQueuedCompletionStatus( completionPort, &bytesTrans, (PULONG_PTR)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE );
		if ( !handleInfo || !ioInfo )
			continue;

		sock = handleInfo->hClntSock;

		if ( ioInfo->rwMode == READ )
		{
			if ( bytesTrans == 0 )    // EOF Àü¼Û ½Ã
			{
				cout << "[ Close ] SOCKET is " << sock << endl;
				closesocket( sock );

				continue;
			}

			string message = ioInfo->wsaBuf.buf;
			message = message.substr( 0, bytesTrans );

			cout << "[ " << sock << " ] " << message;

			memset( &( ioInfo->overlapped ), 0, sizeof( OVERLAPPED ) );
			ioInfo->wsaBuf.len = bytesTrans;
			ioInfo->rwMode = WRITE;
			WSASend( sock, &( ioInfo->wsaBuf ),
				1, NULL, 0, &( ioInfo->overlapped ), NULL );

			ioInfo = new PER_IO_DATA;
			memset( &( ioInfo->overlapped ), 0, sizeof( OVERLAPPED ) );
			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->rwMode = READ;
			WSARecv( sock, &( ioInfo->wsaBuf ),
				1, NULL, &flags, &( ioInfo->overlapped ), NULL );
		}
		else
		{
			cout << "message echo------" << endl;
		}
	}
	return 0;
}