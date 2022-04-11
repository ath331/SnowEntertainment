#include "pch.h"
#include "ServerPacketHandler.h"

PacketHandlerFunc GPacketHandler[ UINT16_MAX ];

bool Handle_INVALID( PacketSessionRef& session, BYTE* buffer, int32 len )
{
	return false;
}

bool Handle_S_LOGIN( PacketSessionRef& session, Protocol::S_LOGIN& pkt )
{
	if ( !pkt.success() )
		return false;

	if ( !pkt.players().size() )
	{
		// 캐릭터 생성
	}

	Protocol::C_ENTER_GAME enterGamePkt;
	enterGamePkt.set_playerindex( 0 );

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer( enterGamePkt );
	session->Send( sendBuffer );

	return true;
}

bool Handle_S_ENTER_GAME( PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt )
{

	return true;
}

bool Handle_S_CHAT( PacketSessionRef& session, Protocol::S_CHAT& pkt )
{
	cout << pkt.msg() << endl;

	return true;
}
