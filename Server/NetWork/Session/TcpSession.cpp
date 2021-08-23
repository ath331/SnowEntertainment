#include "TcpSession.h"
#include "..//..//Core/LogCustom/Log.h"

#include <iostream>

#pragma warning(disable:4996)

using namespace std;

SOCKET TcpSession::GetClietnSock()
{
	return _sock;
}

void TcpSession::_Close()
{
	if ( !shutdown( _sock, SD_SEND ) )
	{
		WARNING_LOG("Client Half-Close", WSAGetLastError() );
		closesocket( _sock );
	}

	cout << "[ Close ] SOCKET is " << _sock << endl;
}

void TcpSession::_PostRecv()
{
	//TODO : offset을 만들어서 기존에 받았던 데이터에 이어서 받기.

	WSARecv( _sock, &( _recvOverlapped.wsaBuf ),
		1, NULL, &_recvFlag, &( _recvOverlapped.overlapped ), NULL );
}

void TcpSession::_PostSend( DWORD bytesTrans )
{
	string message = "Test \n";
	strcpy( _sendOverlapped.buffer, message.c_str() );

	_sendOverlapped.wsaBuf.len = (ULONG)message.length();
	_sendOverlapped.iocpMode = EIocpMode::IOCP_SEND;
	WSASend( _sock, &( _sendOverlapped.wsaBuf ),
		1, NULL, 0, &( _sendOverlapped.overlapped ), NULL );
}


void TcpSession::ProcessRecvForIOCP( DWORD bytesTrans )
{
	if ( bytesTrans == 0 )    // EOF 전송 시
	{
		_Close();

		return;
	}

	//TCP 수신 특성 생각하기

	_PostRecv();
	_PostSend( bytesTrans );
}

void TcpSession::ProcessSendForIOCP()
{
	COMMON_LOG( "message echo------" );
}