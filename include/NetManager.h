#ifndef _NET_MANAGER_H_
#define _NET_MANAGER_H_

#include "User.h"
#include "WorldPacket.h"
#include "Singleton.h"

class NetManager : public Singleton<NetManager>
{
public:
    void Init();
    void StartListen();
    void Loop();
    // int WaitEvent();
    void AddClient(int fd);
	bool SendPacketToUser(const User &user, WorldPacket *packet);
private:
    struct sockaddr_in serverAddr;	// 服务器端的serverAddr信息
	int listener;					// 创建监听的socket
	int epfd;						// epoll_creat创建后的返回值
    
	struct epoll_event events[EPOLL_SIZE];  // epoll 事件队列
};

#define sNetMgr NetManager::getSingleton()

#endif