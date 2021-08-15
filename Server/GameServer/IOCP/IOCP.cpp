#include "IOCP.h"
#include <iostream>
#include <stdlib.h>
#include <process.h>
#include <windows.h>

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
		cout << "new _ioThreadManager error" << endl;
	}

	for ( unsigned short i = 0; i < _iocpThreadCount; i++ )
	{
		_ioThreadManager->Add( (HANDLE)_beginthreadex( NULL, 0, ProcessIocp, this, 0, NULL ) );
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

	_acceptManager = new AcceptManager( _servSock );
	if ( !_acceptManager )
	{
		cout << "new accept error" << endl;
		exit( 1 );
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
	ClientSocketDataPtr clientData = nullptr;
	OverlappedCustomPtr ioInfo = nullptr;
	DWORD flags = 0;

	while ( 1 )
	{
		GetQueuedCompletionStatus( iocpObject->_completionPort, &bytesTrans, (PULONG_PTR)&clientData, (LPOVERLAPPED*)&ioInfo, INFINITE );
		if ( !clientData || !ioInfo )
			continue;

		sock = clientData->hClntSock;

		if ( ioInfo->iocpMode == EIocpMode::ACCEPT )
		{
			iocpObject->_acceptManager->ProcessForIOCP( iocpObject->_completionPort, clientData );
		}
		else if ( ioInfo->iocpMode == EIocpMode::RECV )
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
			ioInfo->iocpMode = EIocpMode::SEND;
			WSASend( sock, &( ioInfo->wsaBuf ),
				1, NULL, 0, &( ioInfo->overlapped ), NULL );


			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->iocpMode = EIocpMode::RECV;
			WSARecv( sock, &( ioInfo->wsaBuf ),
				1, NULL, &flags, &( ioInfo->overlapped ), NULL );
		}
		else if( ioInfo->iocpMode == EIocpMode::SEND )
		{
			cout << "message echo------" << endl;
		}
	}
	return 0;
}