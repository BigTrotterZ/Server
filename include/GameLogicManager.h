#ifndef _GAMELOGIC_MSNSGER_H_
#define _GAMELOGIC_MSNSGER_H_

#include "User.h"
#include "WorldPacket.h"

class GameLogicManager : public Singleton<GameLogicManager>
{
	bool SendPacketToUser(const User &user, WorldPacket *packet);
};

#define sGLMgr GameLogicManager::getSingleton()

#endif