#include "PacketDispatcher.h"
#include "..//BasePacket.h"
#include "..//Pkt/PktNameTransPacket.h"

void PacketDispatcher::MakePacketFromRecvBuf( char* recvBuf, PacketHeader pktHeader )
{
	if ( !recvBuf )
		return;

	int pktSize = pktHeader.size;
	if ( !pktSize )
		return;

	BasePacketPtr packet = PktNameTransPacket( pktHeader.packetName );
	memcpy( (void *)&packet, recvBuf, pktSize );
	if ( !packet )
		return;


}
