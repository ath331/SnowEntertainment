#include "PacketDispatcher.h"
#include "..//BasePacket.h"
#include "..//Pkt/PktNameTransPacket.h"

// 패킷 처리 스레드로 패킷을 전달
void PacketDispatcher::OnPacketDelivery( BasePacket* packet )
{
	BasePacket* pkt = PktNameTransPacket( packet->_GetPacketName() );


}