#pragma once

#include <memory>
#include <WinSock2.h>
#include "Enum.h"

enum class EIocpMode : short
{
	IOCP_ACCEPT,
	IOCP_RECV,
	IOCP_SEND,

	IOCP_MAX
};

class OverlappedCustom
{
public:
	OverlappedCustom()
	{
		memset( &( overlapped ), 0, sizeof( OVERLAPPED ) );
		memset( buffer, 0, sizeof( buffer ) );
		iocpMode = EIocpMode::IOCP_MAX;
		wsaBuf.len = BUF_SIZE;
		wsaBuf.buf = buffer;
		clientSock = 0;
		len = 0;
	}

	~OverlappedCustom() {};

public:
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[ BUF_SIZE ];
	EIocpMode iocpMode;
	SOCKET clientSock;
	DWORD len;
};

using OverlappedCustomPtr = std::shared_ptr< OverlappedCustom >;