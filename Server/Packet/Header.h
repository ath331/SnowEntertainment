#pragma once

#include "..//GameServer/Enum.h"
#include <string>

struct PacketHeader
{
	int size = 0;      //��Ŷ Size

	std::string packetName = "";
};