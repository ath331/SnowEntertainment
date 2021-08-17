#include "ListenManager.h"
#include "../../Core/LogCustom/Log.h"

#include <iostream>

using namespace std;

void ListenManager::Listen( SOCKET sock )
{
	if ( listen( sock, 5 ) != 0 )
	{
		ERROR_LOG( "listen() error!", WSAGetLastError() );
	}
}