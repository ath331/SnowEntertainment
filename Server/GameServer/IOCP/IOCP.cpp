#include "IOCP.h"
#include <winsock2.h>
#include <windows.h>

IOCP::IOCP() :
	CConfigParser("../Ini/GameServer.ini")
{
	_serverIP   = GetString( "IP"   );
	_serverPORT = GetString( "PORT" );


}

IOCP::~IOCP()
{

}

void IOCP::Run()
{

}
