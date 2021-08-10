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

	void Run();

private:
	std::string _serverIP;
	std::string _serverPORT;

};

