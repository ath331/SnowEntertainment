#pragma once

#include <string>
#include "..//PacketConnect.h"

// ��Ŷ�̸����� �ش� ��Ŷ�� ����
BasePacketPtr PktNameTransPacket( std::string pktName )
{
	if ( pktName == "PacketConnect" ) return std::make_shared< PacketConnect >();

	else 
		return nullptr;
}