#include "GameLogicManager.h"
#include "zzLog.h"

bool GameLogicManager::SendPacketToUser(const User & user, WorldPacket * packet)
{
	// 向客户端发送登录成功信息
	int clientfd = user.GetClientFd();
//	char send_buf[BUF_SIZE];
//	bzero(send_buf, BUF_SIZE);
//	memcpy(send_buf, packet->contents(), packet->size());
	int ret = send(clientfd, packet->contents(), packet->size(), 0);
	if(ret < 0)
	{
		return false;
	}
	
	return true;
}