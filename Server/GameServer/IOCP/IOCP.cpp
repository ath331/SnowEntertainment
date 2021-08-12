#include "IOCP.h"
#include <iostream>
#include <stdlib.h>
#include <process.h>
#include <windows.h>

using namespace std;

unsigned int WINAPI EchoThreadMain( LPVOID CompletionPortIO );

IOCP::IOCP() :
	CConfigParser("../Ini/GameServer.ini")
{
	_serverIP        = GetString( "SERVER_IP"         );
	_serverPORT      = GetString( "SERVER_PORT"       );
	_iocpThreadCount = GetInt   ( "IOCP_THREAD_COUNT" );

	_ioInfo     = nullptr;
	_handleInfo = nullptr;

	SOCKADDR_IN servAdr;

	cout << "[ IOCP Setting ]" << endl;
	cout << "IP   : "          << _serverIP << endl;
	cout << "PORT : "          << _serverPORT << endl;
	cout << "IoThreadCount : " << _iocpThreadCount << endl;

	if ( WSAStartup( MAKEWORD( 2, 2 ), &_wsaData ) != 0 )
	{
		cout << "WSAStartup() error!" << endl;
		exit(1);
	}

	_completionPort = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );

	for ( unsigned short i = 0; i < _iocpThreadCount; i++ )
		_beginthreadex( NULL, 0, EchoThreadMain, (LPVOID)_completionPort, 0, NULL );

	_servSock = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	memset( &servAdr, 0, sizeof( servAdr ) );
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl( INADDR_ANY );
	servAdr.sin_port = htons( (u_short)atoi( _serverPORT.c_str() ) );

	if ( bind( _servSock, (SOCKADDR*)& servAdr, sizeof( servAdr ) ) != 0 )
	{
		cout << "bind() error!" << endl;
		exit( 1 );
	}

	if ( listen( _servSock, 5 ) != 0 )
	{
		cout << "listen() error!" << endl;
		exit( 1 );
	}
}

IOCP::~IOCP()
{

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

		cout << "[ Accet ] SOCKET is "<< hClntSock << endl;

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