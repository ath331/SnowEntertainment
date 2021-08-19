#include "IOCP.h"
#include <iostream>
#include <stdlib.h>
#include <process.h>
#include <windows.h>

#include "LogCustom/Log.h"

#include "../NetWork/Accept/AcceptManager.h"
#include "../NetWork/Bind/BindManager.h"
#include "../NetWork/Listen/ListenManager.h"

#include "../OverlappedCustom.h"
#include "../Client/ClientManager.h"

#include "../Thread/ThreadManager.h"

using namespace std;


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
		ERROR_LOG( "new _ioThreadManager error", WSAGetLastError() );
	}

	for ( unsigned short i = 0; i < _iocpThreadCount; i++ )
	{
		_ioThreadManager->Add( (HANDLE)_beginthreadex( NULL, 0, ProcessIocp, this, 0, NULL ) );
	}

	_servSock = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	CreateIoCompletionPort( (HANDLE)_servSock, _completionPort, NULL, 0 );

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

	if ( _acceptManager )
		delete _acceptManager;
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
		ERROR_LOG( "new Bind error", WSAGetLastError() );
	}
	_bindManager->Bind( _servSock );

	_listenManager = new ListenManager;
	if ( !_listenManager )
	{
		ERROR_LOG( "new Listen error", WSAGetLastError() );
	}
	_listenManager->Listen( _servSock );

	_acceptManager = new AcceptManager( _servSock, _completionPort );
	if ( !_acceptManager )
	{
		ERROR_LOG( "new Accept error", WSAGetLastError() );
	}
}

void IOCP::Run()
{
	cout << endl <<"[ ----------SERVER RUN---------- ]" << endl;

	_acceptManager->Accept();
}

void IOCP::Wait()
{
	int threadCount = _ioThreadManager->GetThreadCount();
	vector< HANDLE >* threadVecPtr = _ioThreadManager->GetThreadVecPtr();

	WaitForMultipleObjects( threadCount, threadVecPtr->data(), true, INFINITE );
}

unsigned int WINAPI IOCP::ProcessIocp( LPVOID iocpPtr )
{
	IOCP* iocpObject = (IOCP*)iocpPtr;
	SOCKET sock;
	DWORD bytesTrans;
	OverlappedCustom* ioInfo = nullptr;
	DWORD flags = 0;

	while ( 1 )
	{
		GetQueuedCompletionStatus( iocpObject->_completionPort, &bytesTrans, &sock, (LPOVERLAPPED*)&ioInfo, INFINITE );

		if ( !ioInfo )
			continue;

		if ( ioInfo->iocpMode == EIocpMode::IOCP_ACCEPT )
		{
			iocpObject->_acceptManager->ProcessForIOCP( ioInfo->clientSock );
		}
		else if ( ioInfo->iocpMode == EIocpMode::IOCP_RECV )
		{
			if ( bytesTrans == 0 )    // EOF Àü¼Û ½Ã
			{
				cout << "[ Close ] SOCKET is " << sock << endl;
				closesocket( sock );

				if (!ioInfo)
					delete(ioInfo);

				continue;
			}

			string message = ioInfo->wsaBuf.buf;
			message = message.substr( 0, bytesTrans );

			cout << "[ " << sock << " ] " << message;

			OverlappedCustom* sendIoInfo = new OverlappedCustom;
			memset( &(sendIoInfo->overlapped ), 0, sizeof( OVERLAPPED ) );
			sendIoInfo->wsaBuf.len = bytesTrans;
			sendIoInfo->iocpMode = EIocpMode::IOCP_SEND;
			WSASend( sock, &(sendIoInfo->wsaBuf ),
				1, NULL, 0, &(sendIoInfo->overlapped ), NULL );


			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->iocpMode = EIocpMode::IOCP_RECV;
			WSARecv( sock, &( ioInfo->wsaBuf ),
				1, NULL, &flags, &( ioInfo->overlapped ), NULL );

		}
		else if( ioInfo->iocpMode == EIocpMode::IOCP_SEND )
		{
			COMMON_LOG( "message echo------" );
		}

		if (!ioInfo)
			delete( ioInfo );
	}
	return 0;
}