#include "TcpSession.h"
#include "..//..//Core/LogCustom/Log.h"

#include <cstring>
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

void TcpSession::_PostRecv( size_t size )
{
	strncat( &_recvTempBuf[ _recvOffset - 1 ], _recvOverlapped.wsaBuf.buf, size );

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

	if ( _recvOffset < 4 ) //���� �׽�Ʈ�� ���� �ӽ� ���� 3���� ����
	{
		_PostRecv( bytesTrans );
		return;
	}

	//TCP ���� Ư�� �����ϱ�//

	_PostSend( bytesTrans );
	_PostRecv( bytesTrans );

	_MoveMemoryRecvBuf( 3 );
	_recvOffset -= 3; //���� �׽�Ʈ�� ���� �ӽ� ����. ��Ŷ���̸�ŭ ���������� ��Ŷũ�⸦ ���ش�.
}

void TcpSession::ProcessSendForIOCP()
{
	COMMON_LOG( "message echo------" );
}


//_recvTempBuf�� size��ŭ ����� �Լ�
void TcpSession::_MoveMemoryRecvBuf( size_t size )
{
	if ( !_recvTempBuf )
	{
		WARNING_LOG("_recvTempBuf is NULL ( erroNo is 0 )", 0);
		return;
	}

	if ( _recvOffset + size > BUF_SIZE )
	{
		ERROR_LOG( "size is Over BUF_SIZE  ( erroNo is 0 )", 0 );
	}

	memmove( _recvTempBuf, (const void*)&_recvTempBuf[ size ], _recvOffset - size );
}