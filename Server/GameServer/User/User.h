#pragma once
#include <memory>
#include "..//..//Packet/PacketDispatcher/PacketDispatcher.h"

class TcpSession;

class User
{
public:
	User();
	User( TcpSession* session );

	~User();

private:
	TcpSession* _session;

public:
	PacketDispatcher packetDispatcher;

};

typedef std::shared_ptr< User* > UserPtr;