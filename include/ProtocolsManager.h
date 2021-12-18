#ifndef _PROTOCOLSMGR_H_
#define _PROTOCOLSMGR_H_
#include "Common.h"
#include "Singleton.h"
#include "WorldPacket.h"
#include "User.h"
#include "ProtocolDispatch.h"
#include <unordered_map>
#include <functional>

#define PROTOC_ID(x, y) (x) << 8 + (y)

typedef std::function< bool(WorldPacket *)> RecvProtocFunction;

class ProtocolsManager : public Singleton<ProtocolsManager>
{
public:
	// TODO 错误码枚举
	int RegistProtocol(uint16 protoc_id, const RecvProtocFunction &protoc_event)
	{
		// 检测
		protocol_list.emplace(std::pair<uint16, RecvProtocFunction>(protoc_id, protoc_event));
		Log_Debug("regist; all %u", protocol_list.size());
		return 0;
	}
	int Exec(uint16 protoc_id, WorldPacket* packet){
		//TOTO 检测协议ID是否注册
		if(protocol_list.find(protoc_id) == protocol_list.end())
		{
			Log_Debug("no such protocol!");
			return -1;
		}
		protocol_list[protoc_id](packet);
		return 0;
	}
	bool CreateUserLoginPacket(WorldPacket *packet, const uint8 status);
private:
	std::unordered_map<uint16, RecvProtocFunction> protocol_list;
};

// TestClass testObj;
//     Functional = std::bind(&TestClass::ClassMember, testObj, std::placeholders::_1);

#define sProtocolsMgr ProtocolsManager::getSingleton()

#endif