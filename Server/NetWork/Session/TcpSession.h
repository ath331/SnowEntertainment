#pragma once

#include "..//..//GameServer/User/User.h"
#include "../../GameServer/OverlappedCustom.h"
#include <winsock2.h>

class AcceptManager;
class TcpSession
{
public:
	TcpSession( SOCKET sock ) :
		_sock( sock )
	{
		_recvOverlapped.Init( EIocpMode::IOCP_RECV );
		_recvOverlapped.wsaBuf.len = BUF_SIZE;
		_recvOverlapped.wsaBuf.buf = _recvBuf;
		_recvFlag      = 0;
		_recvOffset    = 0;

		_sendOverlapped.Init( EIocpMode::IOCP_SEND );
		_sendOverlapped.wsaBuf.len = BUF_SIZE;
		_sendOverlapped.wsaBuf.buf = _sendBuf;

		_user = std::make_shared< User >( this );
	}

	~TcpSession() {}

	SOCKET GetClietnSock();

public:
	void ProcessRecvForIOCP( DWORD bytesTrans );
	void ProcessSendForIOCP();

private:
	SOCKET _sock; //clientSock

	OverlappedCustom _recvOverlapped;
	char  _recvBuf[ BUF_SIZE ]     = { 0, };   //WSABUF�� �����͸� �޴� ����
	char  _recvTempBuf[ BUF_SIZE ] = { 0, };   //������ �����Ͱ� ����Ǵ� �ӽ� ����
	DWORD _recvFlag;
	DWORD _recvOffset;  //���ִ� ������ ũ��

	OverlappedCustom _sendOverlapped;
	char _sendBuf[ BUF_SIZE ];

private:
	UserPtr _user;     // �ش� ������ ����ϴ� ����

private:
	void _Close();
	void _PostRecv( size_t size = 0 );
	void _PostSend( DWORD bytesTrans );

	void _CatStr( size_t size );
	void _MoveMemoryRecvBuf( size_t size );

private:
	friend AcceptManager;

};

