#include "BindManager.h"

BindManager::BindManager( string port )
{
	memset( &_servAdr, 0, sizeof( _servAdr ) );
	_servAdr.sin_family = AF_INET;
	_servAdr.sin_addr.s_addr = htonl( INADDR_ANY );
	_servAdr.sin_port = htons( (u_short)atoi( port.c_str() ) );
}

BindManager::~BindManager()
{

}

void BindManager::Bind( SOCKET sock )
{
	if ( bind( sock, (SOCKADDR*)& _servAdr, sizeof( _servAdr ) ) != 0 )
	{
		cout << "bind() error!" << endl;
		exit( 1 );
	}
}

