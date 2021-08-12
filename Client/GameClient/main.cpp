#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#include <iostream>

using namespace std;

#define BUF_SIZE 1024
void ErrorHandling( const char* message );

int main( int argc, char* argv[] )
{
	WSADATA wsaData;
	SOCKET hSocket;
	char message[ BUF_SIZE ];
	int strLen;
	SOCKADDR_IN servAdr;

	std::string defaultIP   = "127.0.0.1";
	std::string defaultPORT = "9999";

	if ( argc != 3 ) 
	{
		cout << "Not Setting IP or PORT."          << endl;
		cout << "Serring defaultIP and defaultPORT" << endl;

		argv[ 1 ] = (char*)defaultIP.c_str();
		argv[ 2 ] = (char*)defaultPORT.c_str();
	}

	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 )
		ErrorHandling( "WSAStartup() error!" );

	hSocket = socket( PF_INET, SOCK_STREAM, 0 );
	if ( hSocket == INVALID_SOCKET )
		ErrorHandling( "socket() error" );

	memset( &servAdr, 0, sizeof( servAdr ) );
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr( argv[ 1 ] );
	servAdr.sin_port = htons( atoi( argv[ 2 ] ) );

	if ( connect( hSocket, (SOCKADDR*)& servAdr, sizeof( servAdr ) ) == SOCKET_ERROR )
		ErrorHandling( "connect() error!" );
	else
		puts( "Connected..........." );

	while ( 1 )
	{
		fputs( "Input message(Q to quit): ", stdout );
		fgets( message, BUF_SIZE, stdin );

		if ( !strcmp( message, "q\n" ) || !strcmp( message, "Q\n" ) )
			break;

		send( hSocket, message, strlen( message ), 0 );
		strLen = recv( hSocket, message, BUF_SIZE - 1, 0 );
		message[ strLen ] = 0;
		printf( "Message from server: %s", message );
	}

	closesocket( hSocket );
	WSACleanup();
	return 0;
}

void ErrorHandling( const char* message )
{
	fputs( message, stderr );
	fputc( '\n', stderr );
	exit( 1 );
}