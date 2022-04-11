#include "pch.h"
#include "ClientPacketHandler.h"
#include "Player.h"
#include "Room.h"
#include "GameSession.h"

PacketHandlerFunc GPacketHandler[ UINT16_MAX ];

// 직접 컨텐츠 작업자

bool Handle_INVALID( PacketSessionRef& session, BYTE* buffer, int32 len )
{
	return false;
}

bool Handle_C_LOGIN( PacketSessionRef& session, Protocol::C_LOGIN& pkt )
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>( session );

	Protocol::S_LOGIN loginPkt;
	loginPkt.set_success( true );

	static Atomic< uint64 > idGenerator = 1;

	{
		auto player = loginPkt.add_players();
		player->set_name( u8"DB에서 긁어온 이름1" );
		player->set_playertype( Protocol::PLAYER_TYPE_KNIGHT );

		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->name = player->name();
		playerRef->type = player->playertype();
		playerRef->ownerSession = gameSession;

		gameSession->_players.push_back( playerRef );
	}
	{
		auto player = loginPkt.add_players();
		player->set_name( u8"DB에서 긁어온 이름2" );
		player->set_playertype( Protocol::PLAYER_TYPE_MAGE );

		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->name = player->name();
		playerRef->type = player->playertype();
		playerRef->ownerSession = gameSession;

		gameSession->_players.push_back( playerRef );
	}

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer( loginPkt );
	session->Send( sendBuffer );

	return true;
}

bool Handle_C_ENTER_GAME( PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt )
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>( session );

	uint64 index = pkt.playerindex();

	gameSession->_currentPlayer = gameSession->_players[ index ];
	gameSession->_room = GRoom;

	GRoom->DoAsync( &Room::Enter, gameSession->_currentPlayer );

	Protocol::S_ENTER_GAME enterGamePkt;
	enterGamePkt.set_success( true );
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer( enterGamePkt );
	gameSession->_currentPlayer->ownerSession->Send( sendBuffer );

	return true;
}

bool Handle_C_CHAT( PacketSessionRef& session, Protocol::C_CHAT& pkt )
{
	cout << pkt.msg() << endl;

	Protocol::S_CHAT chatPkt;
	chatPkt.set_msg( pkt.msg() );
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer( chatPkt );

	GRoom->DoAsync( &Room::Broadcast, sendBuffer );

	return false;
}
