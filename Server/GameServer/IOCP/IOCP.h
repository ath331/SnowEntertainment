#pragma once
#include "../Core/Parser/CConfigParser.h"
#include "../Core/SingleTon/UxSingleTon.h"

class IOCP :
	public CConfigParser,
	public UxSingleTon< IOCP >
{
public:
	IOCP();
	~IOCP();

	void Test()
	{
		std::cout << "server IP   : " << _serverIP   << std::endl;
		std::cout << "server PORT : " << _serverPORT << std::endl;
	}

private:
	std::string _serverIP;
	std::string _serverPORT;

};

