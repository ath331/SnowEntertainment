#pragma once

#include <memory>
#include <WinSock2.h>
#include "Enum.h"

class OverlappedCustom
{
public:
	OverlappedCustom()
	{
		memset( &( overlapped ), 0, sizeof( OVERLAPPED ) );
		iocpMode = EIocpMode::MAX;
		wsaBuf.len = BUF_SIZE;
		wsaBuf.buf = buffer;
		clientSock = 0;
	}

	~OverlappedCustom() {};

public:
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[ BUF_SIZE ];
	EIocpMode iocpMode;
	SOCKET clientSock;
};

using OverlappedCustomPtr = std::shared_ptr< OverlappedCustom >;