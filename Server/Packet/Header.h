#pragma once

#include "..//GameServer/Enum.h"

struct PakcetHeader
{
	unsigned short size         = 0;      //��Ŷ Size
	char packetData[ BUF_SIZE ] = { 0, }; //��Ŷ Data
};