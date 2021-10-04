#include "TcpSession.h"
#include <cstring>
#include <iostream>

#include "..//..//Core/LogCustom/Log.h"
#include "..//..//Packet/BasePacket.h"

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

void TcpSession::_PostRecv( size_t size )
{
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
	_CatStr( bytesTrans );

	if ( _recvOffset < sizeof( PakcetHeader ) )
	{
		_PostRecv( bytesTrans );

		return;
	}

	PakcetHeader packetHeader;
	memcpy( &packetHeader, _recvTempBuf, sizeof( PakcetHeader ) );

	if ( _recvOffset < packetHeader.size )
	{
		_CatStr( bytesTrans );
		_PostRecv( bytesTrans );

		return;
	}

	//TODO : 임시버퍼에서 패킷을 만들 데이터만 추출

	_CatStr( bytesTrans );

	_MoveMemoryRecvBuf( 5 ); //패킷 크기 5으로 가정
	_recvOffset -= 5; //동작 테스트를 위해 임시 길이. 패킷길이만큼 수신했으면 패킷크기를 뺴준다.

	_PostRecv( bytesTrans );
}

void TcpSession::ProcessSendForIOCP()
{
	COMMON_LOG( "message echo------" );
}

//임시 버퍼에 size만큼 데이터를 이어붙이는 함수
void TcpSession::_CatStr( size_t size )
{
	strncat( _recvTempBuf, _recvOverlapped.wsaBuf.buf, size );
}

//_recvTempBuf를 size만큼 떙기는 함수
void TcpSession::_MoveMemoryRecvBuf( size_t size )
{
	if ( !_recvTempBuf )
	{
		WARNING_LOG("_recvTempBuf is NULL ( erroNo is Empty )", 0);
		return;
	}

	if ( _recvOffset + size > BUF_SIZE )
	{
		ERROR_LOG( "size is Over BUF_SIZE  ( erroNo is Empty )", 0 );
	}

	memmove( _recvTempBuf, (const void*)&_recvTempBuf[ size ], _recvOffset - size );
	memset( &_recvTempBuf[ _recvOffset - size ], 0, sizeof(_recvTempBuf) );

}