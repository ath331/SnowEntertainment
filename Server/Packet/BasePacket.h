#pragma once
#include "Header.h"

struct BasePacket
{
	PakcetHeader header;

protected:
	virtual void OnPacketHandler() = 0;
};

