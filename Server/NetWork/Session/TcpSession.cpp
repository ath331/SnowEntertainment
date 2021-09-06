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
	strcat( _recvTempBuf, _recvOverlapped.wsaBuf.buf );

	if ( SOCKET_ERROR == WSARecv(
		_sock,
		&_recvOverlapped.wsaBuf,
		1,
		NULL,
		&_recvFlag,
		&( _recvOverlapped.overlapped ),
		NULL ) )
	{
		int error = WSAGetLastError();
		if ( error != WSA_IO_PENDING )
		{
			WARNING_LOG( "WSARECV ERROR", error );
		}
	}
}

void TcpSession::_PostSend( DWORD bytesTrans )
{
	//TOOD : 이미 전송중일떄는 바로 전송이 되나?? 전송중인 상태와 전송대기 상태를 만들어야하나?

	string message = "Test \n";
	strcpy( _sendOverlapped.buffer, message.c_str() );
	_sendOverlapped.wsaBuf.len = (ULONG)message.length();

	if ( SOCKET_ERROR == WSASend(
		_sock,
		&( _sendOverlapped.wsaBuf ),
		1,
		NULL,
		0,
		&( _sendOverlapped.overlapped ),
		NULL ) )
	{
		int error = WSAGetLastError();
		if ( error != WSA_IO_PENDING )
		{
			WARNING_LOG( "WSAEND ERROR", error );
		}
	}
}


void TcpSession::ProcessRecvForIOCP( DWORD bytesTrans )
{
	if ( bytesTrans == 0 )    // EOF 전송 시
	{
		_Close();

		return;
	}

	_recvOffset += bytesTrans;

	if ( _recvOffset < 3 ) //동작 테스트를 위해 임시 길이 3으로 설정
	{
		_PostRecv();
		return;
	}

	//TCP 수신 특성 생각하기//

	_PostRecv();
	_PostSend( bytesTrans );

	_recvOffset -= _recvOffset; //동작 테스트를 위해 임시 길이. 패킷길이만큼 수신했으면 패킷크기를 뺴준다.
}

void TcpSession::ProcessSendForIOCP()
{
	COMMON_LOG( "message echo------" );
}