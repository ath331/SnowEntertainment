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
	//TOOD : �̹� �������ϋ��� �ٷ� ������ �ǳ�?? �������� ���¿� ���۴�� ���¸� �������ϳ�?

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
	if ( bytesTrans == 0 )    // EOF ���� ��
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

	//TODO : �ӽù��ۿ��� ��Ŷ�� ���� �����͸� ����

	_CatStr( bytesTrans );

	_MoveMemoryRecvBuf( 5 ); //��Ŷ ũ�� 5���� ����
	_recvOffset -= 5; //���� �׽�Ʈ�� ���� �ӽ� ����. ��Ŷ���̸�ŭ ���������� ��Ŷũ�⸦ ���ش�.

	_PostRecv( bytesTrans );
}

void TcpSession::ProcessSendForIOCP()
{
	COMMON_LOG( "message echo------" );
}

//�ӽ� ���ۿ� size��ŭ �����͸� �̾���̴� �Լ�
void TcpSession::_CatStr( size_t size )
{
	strncat( _recvTempBuf, _recvOverlapped.wsaBuf.buf, size );
}

//_recvTempBuf�� size��ŭ ����� �Լ�
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