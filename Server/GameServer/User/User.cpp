#include "User.h"
#include "..//NetWork/Session/TcpSession.h"

User::User()
{
	_session = nullptr;
}

User::User( TcpSession* session )
{
	_session = session;
}

User::~User()
{
}