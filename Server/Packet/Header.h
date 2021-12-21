#pragma once

#include "..//GameServer/Enum.h"
#include <string>

struct PacketHeader
{
	int size         = 0;      //패킷 Size
	char packetData[ BUF_SIZE ] = { 0, }; //패킷 Data

	std::string packetName = "";
};