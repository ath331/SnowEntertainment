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
	//TODO : offset�� ���� ������ �޾Ҵ� �����Ϳ� �̾ �ޱ�.

	WSARecv( _sock, &( _recvOverlapped.wsaBuf ),
		1, NULL, &_recvFlag, &( _recvOverlapped.overlapped ), NULL );
}

void TcpSession::_PostSend( DWORD bytesTrans )
{
	//TOOD : �̹� �������ϋ��� �ٷ� ������ �ǳ�?? �������� ���¿� ���۴�� ���¸� �������ϳ�?

	string message = "Test \n";
	strcpy( _sendOverlapped.buffer, message.c_str() );
	_sendOverlapped.wsaBuf.len = (ULONG)message.length();

	WSASend( _sock, &( _sendOverlapped.wsaBuf ),
		1, NULL, 0, &( _sendOverlapped.overlapped ), NULL );
}


void TcpSession::ProcessRecvForIOCP( DWORD bytesTrans )
{
	if ( bytesTrans == 0 )    // EOF ���� ��
	{
		_Close();

		return;
	}

	//TCP ���� Ư�� �����ϱ�

	_PostRecv();
	_PostSend( bytesTrans );
}

void TcpSession::ProcessSendForIOCP()
{
	COMMON_LOG( "message echo------" );
}