#pragma once
#include "JobQueue.h"

class Room : public JobQueue
{
public:
	// 싱글 스레드인것처럼 작성
	void Enter    ( PlayerRef player         );
	void Leave    ( PlayerRef player         );
	void Broadcast( SendBufferRef sendBuffer );

private:
	map< uint64, PlayerRef > _players;

};

extern shared_ptr<Room> GRoom;