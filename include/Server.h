//服务端类的实现
// 1、init()初始化
// 2、Start()启动服务
// 3、Close()关闭服务
// 4、广播消息给所有客户端函数SendBroadcastMessage()
// 服务端的主循环中每次都会检查并处理EPOLL中的就绪事件，而就绪时间列表主要就是两种类型：新连接
// 或新消息。服务器会一次从就绪事件列表里提取事件进行处理，如果是新连接则accept()然后addfd()，
// 如果是新消息则SendBroadcastMessage()实现聊天功能。
#ifndef _SERVER_H_
#define _SERVER_H_

#include <string>

#include "Common.h"
#include "User.h"
#include "ThreadPoolManager.h"

class Server
{
public:
	// 无参构造
	Server();
	// 初始化服务器设置
	void InitServer();
	// 关闭服务
	void CloseServer();
	// 启动服务
	void StartServer();
private:
	// 广播消息给所有客户端
	int SendBroadcastMessage(int clientfd);
private:
	// struct sockaddr_in serverAddr;	// 服务器端的serverAddr信息
	// int listener;					// 创建监听的socket
	// int epfd;						// epoll_creat创建后的返回值
	std::list<int> clients_list;	// 客户端列表
	std::map<int, UserPtr> user_list;	// 用户列表
	ThreadPoolManager * net_thread_pool_;
};

#endif