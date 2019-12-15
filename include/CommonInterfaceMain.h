#ifndef _COMMON_INTERFACE_MAIN_H_
#define _COMMON_INTERFACE_MAIN_H_

#include "zzLog.h"
#include "GameLogicManager.h"
#include "ProtocolsManager.h"

initialiseSingleton(ZZLog);
initialiseSingleton(GameLogicManager);
initialiseSingleton(ProtocolsManager);

#define CreateLibSingleton( type ) \
  static  type the##type

void CommonInitInterface()
{
	CreateLibSingleton(ZZLog);
	CreateLibSingleton(GameLogicManager);
	CreateLibSingleton(ProtocolsManager);
}

#endif