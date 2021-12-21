#pragma once

#include "..//GameServer/Enum.h"
#include <string>

struct PacketHeader
{
	int size = 0;      //ÆÐÅ¶ Size

	std::string packetName = "";
};