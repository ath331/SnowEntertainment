#include "IOCP.h"

IOCP::IOCP() :
	CConfigParser("../Ini/GameServer.ini")
{
	_serverIP   = GetString( "IP"   );
	_serverPORT = GetString( "PORT" );

}

IOCP::~IOCP()
{

}