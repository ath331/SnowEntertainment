#pragma once

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
		_recvFlag   = 0;
		_recvOffset = 0;

		_sendOverlapped.Init( EIocpMode::IOCP_SEND );
		_sendOverlapped.wsaBuf.len = BUF_SIZE;
		_sendOverlapped.wsaBuf.buf = _sendBuf;
	}

	~TcpSession() {}

	SOCKET GetClietnSock();

public:
	void ProcessRecvForIOCP( DWORD bytesTrans );
	void ProcessSendForIOCP();

private:
	SOCKET _sock; //clientSock

	OverlappedCustom _recvOverlapped;
	char  _recvBuf[ BUF_SIZE ] = { 0, };
	char  _recvTempBuf[ BUF_SIZE ] = { 0, }; //������ �����Ͱ� ����Ǵ� �ӽ� ����
	DWORD _recvFlag;
	DWORD _recvOffset;  //recv�����͸� ���� ��ġ

	OverlappedCustom _sendOverlapped;
	char _sendBuf[ BUF_SIZE ];


private:
	void _Close();
	void _PostRecv();
	void _PostSend( DWORD bytesTrans );

private:
	friend AcceptManager;

};

