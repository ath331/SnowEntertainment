#include <iostream>
#include "IOCP/IOCP.h"
#include "../NetWork/NetWork.h"

//TODO : ���� ��� �����

int main()
{
	NetWork netWork;

	IOCP iocp;
	iocp.Run();

	return 0;
}