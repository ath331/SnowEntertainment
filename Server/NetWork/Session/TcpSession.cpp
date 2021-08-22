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

void TcpSession::ProcessRecvForIOCP( DWORD bytesTrans )
{
	if ( bytesTrans == 0 )    // EOF ���� ��
	{
		_Close();

		return;
	}

	//TCP ���� Ư�� �����ϱ�

	string message = "Test\n";

	OverlappedCustom* sendIoInfo = new OverlappedCustom;
	memset( &( sendIoInfo->overlapped ), 0, sizeof( OVERLAPPED ) );

	strcpy( sendIoInfo->buffer, message.c_str() );

	sendIoInfo->wsaBuf.len = bytesTrans;
	sendIoInfo->iocpMode = EIocpMode::IOCP_SEND;
	WSASend( _sock, &( sendIoInfo->wsaBuf ),
		1, NULL, 0, &( sendIoInfo->overlapped ), NULL );

	_PostRecv();
}

void TcpSession::ProcessSendForIOCP()
{
	
}