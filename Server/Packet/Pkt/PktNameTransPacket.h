#pragma once

#include <string>
#include "..//PacketConnect.h"

// ��Ŷ�̸����� �ش� ��Ŷ�� ����
BasePacket* PktNameTransPacket( std::string pktName )
{
	if ( pktName == "PacketConnect" ) return new PacketConnect;
}