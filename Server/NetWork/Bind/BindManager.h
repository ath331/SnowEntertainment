#pragma once

#include <windows.h>
#include <iostream>

using namespace std;

class BindManager
{
public:
	BindManager( string port );
	~BindManager();

	void Bind( SOCKET sock );

private:
	SOCKADDR_IN _servAdr;

};

