#pragma once

#include <winsock2.h>
#include <memory>

struct ClientSocketData
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
};

using ClientSocketDataPtr = std::shared_ptr< ClientSocketData >;

class ClientManager
{
};

