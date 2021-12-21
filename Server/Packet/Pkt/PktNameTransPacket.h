#pragma once

#include <string>
#include "..//PacketConnect.h"

// 패킷이름으로 해당 패킷을 생성
BasePacketPtr PktNameTransPacket( std::string pktName )
{
	if ( pktName == "PacketConnect" ) return std::make_shared< PacketConnect >();

	else 
		return nullptr;
}