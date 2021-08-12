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
	_serverIP        = GetString( "SERVER_IP"       );
	_serverPORT      = GetString( "SERVER_PORT"     );
	_iocpThreadCount = GetInt   ("IOCP_THREAD_COUNT");

	_ioInfo     = nullptr;
	_handleInfo = nullptr;

	SOCKADDR_IN servAdr;

	if ( WSAStartup( MAKEWORD( 2, 2 ), &_wsaData ) != 0 )
		cout << "WSAStartup() error!" <<endl;

	_hComPort = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );

	for ( unsigned short i = 0; i < _iocpThreadCount; i++ )
		_beginthreadex( NULL, 0, EchoThreadMain, (LPVOID)_hComPort, 0, NULL );

	_servSock = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	memset( &servAdr, 0, sizeof( servAdr ) );
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl( INADDR_ANY );
	servAdr.sin_port = htons( (u_short)atoi( _serverPORT.c_str() ) );

	bind( _servSock, (SOCKADDR*)& servAdr, sizeof( servAdr ) );
	listen( _servSock, 5 );
}

IOCP::~IOCP()
{

}

void IOCP::Run()
{
	unsigned int recvBytes, flags = 0;

	while ( 1 )
	{
		SOCKET hClntSock;
		SOCKADDR_IN clntAdr;
		int addrLen = sizeof( clntAdr );

		hClntSock = accept( _servSock, (SOCKADDR*)& clntAdr, &addrLen );

		_handleInfo = new PER_HANDLE_DATA;
		if ( !_handleInfo )
			continue;

		_handleInfo->hClntSock = hClntSock;
		memcpy( &( _handleInfo->clntAdr ), &clntAdr, addrLen );

		CreateIoCompletionPort( (HANDLE)hClntSock, _hComPort, ( unsigned long long )_handleInfo, 0 );

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
	HANDLE _hComPort = (HANDLE)pComPort;
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_HANDLE_DATA _handleInfo = nullptr;
	LPPER_IO_DATA _ioInfo = nullptr;
	DWORD flags = 0;

	while ( 1 )
	{
		GetQueuedCompletionStatus( _hComPort, &bytesTrans, (PULONG_PTR)&_handleInfo, (LPOVERLAPPED*)&_ioInfo, INFINITE );
		if ( !_handleInfo || !_ioInfo )
			continue;

		sock = _handleInfo->hClntSock;

		if ( _ioInfo->rwMode == READ )
		{
			puts( "message received!" );
			if ( bytesTrans == 0 )    // EOF Àü¼Û ½Ã
			{
				closesocket( sock );
				continue;
			}

			memset( &( _ioInfo->overlapped ), 0, sizeof( OVERLAPPED ) );
			_ioInfo->wsaBuf.len = bytesTrans;
			_ioInfo->rwMode = WRITE;
			WSASend( sock, &( _ioInfo->wsaBuf ),
				1, NULL, 0, &( _ioInfo->overlapped ), NULL );

			_ioInfo = new PER_IO_DATA;
			memset( &( _ioInfo->overlapped ), 0, sizeof( OVERLAPPED ) );
			_ioInfo->wsaBuf.len = BUF_SIZE;
			_ioInfo->wsaBuf.buf = _ioInfo->buffer;
			_ioInfo->rwMode = READ;
			WSARecv( sock, &( _ioInfo->wsaBuf ),
				1, NULL, &flags, &( _ioInfo->overlapped ), NULL );
		}
		else
		{
			puts( "[Server] : " );
		}
	}
	return 0;
}