#include "IOCP.h"
#include <iostream>
#include <stdlib.h>
#include <process.h>
#include <windows.h>

#define BUF_SIZE 100
#define READ	3
#define	WRITE	5

using namespace std;

typedef struct    // socket info
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
} PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

typedef struct    // buffer info
{
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[ BUF_SIZE ];
	int rwMode;    // READ or WRITE
} PER_IO_DATA, * LPPER_IO_DATA;

unsigned int WINAPI EchoThreadMain( LPVOID CompletionPortIO );

IOCP::IOCP() :
	CConfigParser("../Ini/GameServer.ini")
{
	_serverIP   = GetString( "IP"   );
	_serverPORT = GetString( "PORT" );

	WSADATA	wsaData;
	HANDLE hComPort;
	SYSTEM_INFO sysInfo;
	LPPER_IO_DATA ioInfo = nullptr;
	LPPER_HANDLE_DATA handleInfo = nullptr;

	SOCKADDR_IN servAdr;
	unsigned int recvBytes, i, flags = 0;

	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 )
		cout << "WSAStartup() error!" <<endl;

	hComPort = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );
	GetSystemInfo( &sysInfo );

	for ( i = 0; i < sysInfo.dwNumberOfProcessors; i++ )
		_beginthreadex( NULL, 0, EchoThreadMain, (LPVOID)hComPort, 0, NULL );

	_servSock = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	memset( &servAdr, 0, sizeof( servAdr ) );
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl( INADDR_ANY );
	servAdr.sin_port = htons( (u_short)atoi( _serverPORT.c_str() ) );

	bind( _servSock, (SOCKADDR*)& servAdr, sizeof( servAdr ) );
	listen( _servSock, 5 );

	while ( 1 )
	{
		SOCKET hClntSock;
		SOCKADDR_IN clntAdr;
		int addrLen = sizeof( clntAdr );

		hClntSock = accept( _servSock, (SOCKADDR*)& clntAdr, &addrLen );

		handleInfo = new PER_HANDLE_DATA;
		if ( !handleInfo )
			continue;

		handleInfo->hClntSock = hClntSock;
		memcpy( &( handleInfo->clntAdr ), &clntAdr, addrLen );

		CreateIoCompletionPort( (HANDLE)hClntSock, hComPort, (unsigned long long)handleInfo, 0 );

		ioInfo = new PER_IO_DATA;
		if ( !ioInfo )
			continue;

		memset( &( ioInfo->overlapped ), 0, sizeof( OVERLAPPED ) );
		ioInfo->wsaBuf.len = BUF_SIZE;
		ioInfo->wsaBuf.buf = ioInfo->buffer;
		ioInfo->rwMode = READ;

		WSARecv( handleInfo->hClntSock, &( ioInfo->wsaBuf ),
			1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &( ioInfo->overlapped ), NULL );
	}
}

IOCP::~IOCP()
{

}

void IOCP::Run()
{

}

unsigned int WINAPI EchoThreadMain( LPVOID pComPort )
{
	HANDLE hComPort = (HANDLE)pComPort;
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_HANDLE_DATA handleInfo = nullptr;
	LPPER_IO_DATA ioInfo = nullptr;
	DWORD flags = 0;

	while ( 1 )
	{
		GetQueuedCompletionStatus( hComPort, &bytesTrans, (PULONG_PTR)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE );
		if ( !handleInfo || !ioInfo )
			continue;

		sock = handleInfo->hClntSock;

		if ( ioInfo->rwMode == READ )
		{
			puts( "message received!" );
			if ( bytesTrans == 0 )    // EOF Àü¼Û ½Ã
			{
				closesocket( sock );
				continue;
			}

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
			puts( "message sent!" );
		}
	}
	return 0;
}