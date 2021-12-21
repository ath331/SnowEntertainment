#pragma once
#include <memory>
#include "..//..//Packet/PacketDispatcher/PacketDispatcher.h"

class TcpSession;

class User
	: public PacketDispatcher
{
public:
	User();
	User( TcpSession* session );

	~User();

private:
	TcpSession* _session;

};

typedef std::shared_ptr< User > UserPtr;