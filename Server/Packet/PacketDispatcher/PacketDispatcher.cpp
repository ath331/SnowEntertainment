#include "PacketDispatcher.h"
#include "..//BasePacket.h"
#include "..//Pkt/PktNameTransPacket.h"

// ��Ŷ ó�� ������� ��Ŷ�� ����
void PacketDispatcher::OnPacketDelivery( BasePacket* packet )
{
	BasePacket* pkt = PktNameTransPacket( packet->_GetPacketName() );


}