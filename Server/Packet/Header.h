#pragma once

#include "..//GameServer/Enum.h"
#include <string>

struct PacketHeader
{
	int size         = 0;      //��Ŷ Size
	char packetData[ BUF_SIZE ] = { 0, }; //��Ŷ Data

	std::string packetName = "";
};