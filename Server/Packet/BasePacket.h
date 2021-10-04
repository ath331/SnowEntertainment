#pragma once
#include "Header.h"

struct BasePacket
{
	PakcetHeader header;

protected:
	virtual void OnPackethandler() = 0;
};

