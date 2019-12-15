#ifndef _PROTOCOLSMGR_H_
#define _PROTOCOLSMGR_H_
#include "Common.h"
#include "Singleton.h"
#include "WorldPacket.h"
#include "User.h"


class ProtocolsManager : public Singleton<ProtocolsManager>
{
public:
	bool CreateUserLoginPacket(WorldPacket *packet, const uint8 status);
};

#define sProtocolsMgr ProtocolsManager::getSingleton()

#endif