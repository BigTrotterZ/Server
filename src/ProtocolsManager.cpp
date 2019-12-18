#include "ProtocolsManager.h"
#include "ProtocolsID.h"

bool ProtocolsManager::CreateUserLoginPacket(WorldPacket *packet, const uint8 status)
{
	if (!packet)
		return false;
	packet->clear();
	uint16 packlen = 9;
	packet->reserve(packlen);
	packet->SetOpcode(SMSG_LOGIN);

	*packet << uint16(SMSG_LOGIN) << uint16(packlen)
		<< uint8(status)
		<< uint32(time(0)); // 时间戳

	return true;
}