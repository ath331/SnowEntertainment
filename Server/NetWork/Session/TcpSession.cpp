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

	if ( _recvOffset < 3 ) //���� �׽�Ʈ�� ���� �ӽ� ���� 3���� ����
	{
		_PostRecv();
		return;
	}

	//TCP ���� Ư�� �����ϱ�//

	_PostRecv();
	_PostSend( bytesTrans );

	_recvOffset -= _recvOffset; //���� �׽�Ʈ�� ���� �ӽ� ����. ��Ŷ���̸�ŭ ���������� ��Ŷũ�⸦ ���ش�.
}

void TcpSession::ProcessSendForIOCP()
{
	COMMON_LOG( "message echo------" );
}