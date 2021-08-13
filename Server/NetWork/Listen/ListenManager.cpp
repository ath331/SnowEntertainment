#include "ListenManager.h"

#include <iostream>

using namespace std;

void ListenManager::Listen( SOCKET sock )
{
	if ( listen( sock, 5 ) != 0 )
	{
		cout << "listen() error!" << endl;
		exit( 1 );
	}
}