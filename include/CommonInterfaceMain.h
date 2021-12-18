#ifndef _COMMON_INTERFACE_MAIN_H_
#define _COMMON_INTERFACE_MAIN_H_

#include "zzLog.h"
#include "GameLogicManager.h"
#include "ProtocolsManager.h"
#include "NetManager.h"

initialiseSingleton(ZZLog);
initialiseSingleton(GameLogicManager);
initialiseSingleton(ProtocolsManager);
initialiseSingleton(NetManager);

#define CreateLibSingleton( type ) \
  static  type the##type

void CommonInitInterface()
{
	CreateLibSingleton(ZZLog);
	CreateLibSingleton(GameLogicManager);
	CreateLibSingleton(ProtocolsManager);
	CreateLibSingleton(NetManager);
}

#endif