#include <iostream>
#include "IOCP/IOCP.h"
#include "../NetWork/NetWork.h"

//TODO : 공용 헤더 만들기

int main()
{
	NetWork netWork;

	IOCP iocp;
	iocp.Run();

	return 0;
}