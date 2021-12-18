// Server.cpp
#include <iostream>

#include "Server.h"
#include "NetManager.h"
#include "ProtocolsManager.h"

// 服务端类成员函数

// 服务端类构造函数
Server::Server()
{
	// // 初始化服务器地址和端口
	// bzero(&serverAddr, sizeof(serverAddr));
	// serverAddr.sin_family = AF_INET;
	// serverAddr.sin_port = htons(SERVER_PORT);
	// serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// // 初始化socket
	// listener = 0;
	// // epoll fd
	// epfd = 0;
	sNetMgr.Init();
	net_thread_pool_ = new ThreadPoolManager(5);
	// net_thread_pool_(5);
}

// 初始化服务器并启动监听
void Server::InitServer()
{
	sNetMgr.StartListen();
	net_thread_pool_->Start();

	sProtocolsMgr.RegistProtocol(0x0300, DealProtocol);
}

// 关闭服务
void Server::CloseServer()
{
	net_thread_pool_->Stop();
	delete net_thread_pool_;
	// 关闭socket
	// close(listener);

	// 关闭epoll监听
	// close(epfd);
}

// 发送广播消息给所有客户端
int Server::SendBroadcastMessage(int clientfd)
{
	// buf[BUF_SIZE] 接受新消息
	// message[BUF_SIZE] 保存格式化的消息
	char recv_buf[BUF_SIZE];
	char send_buf[BUF_SIZE];
	Msg msg;
	bzero(recv_buf, BUF_SIZE);

	// 接受新消息
	Log.Debug(__FUNC_HEAD__, "read from client[%d]", clientfd);
	int len = recv(clientfd, recv_buf, BUF_SIZE, 0);
	// 清空结构体，把接受到的字符串转换为结构体
	memset(&msg, 0, sizeof(msg));
	memcpy(&msg, recv_buf, sizeof(msg));

	// 判断接受到的消息是私聊还是群聊
	msg.fromID = clientfd;
	if(msg.content[0] == '\\' && isdigit(msg.content[1]))
	{
		msg.type = 1;
		msg.toID = msg.content[1] - '0';
		memcpy(msg.content, msg.content+2, sizeof(msg.content));
	}
	else
		msg.type = 0;

	// 如果客户端关闭了连接
	if(len == 0)
	{
		close(clientfd);

		// 在客户列表中删除该客户端
		clients_list.remove(clientfd);
		Log.Debug(__FUNC_HEAD__, "Client[%d] closed!", clientfd);
		Log.Debug(__FUNC_HEAD__, "now there are [%d] client in the room", clients_list.size());
	}
	else 	// 发送广播消息给所有客户端
	{
		// 判断是否聊天室还有其他客户端
		if(clients_list.size() == 1)
		{
			memcpy(&msg.content, CAUTION, sizeof(msg.content));
			bzero(send_buf, BUF_SIZE);
			memcpy(send_buf, &msg, sizeof(msg));
			send(clientfd, send_buf, sizeof(send_buf), 0);
			return len;
		}

		char format_message[BUF_SIZE];
		// 群聊
		if(msg.type == 0)
		{
			// 格式化发送的消息内容 #define SERVER_MESSAGE "ClientID %d say >> %s"
			sprintf(format_message, SERVER_MESSAGE, clientfd, msg.content);
			memcpy(msg.content, format_message, BUF_SIZE);
			// 遍历客户端列表依次发送消息，需要判断不要给来源客户端发
			std::list<int>::iterator iter = clients_list.begin();
			for(; iter != clients_list.end(); ++iter)
			{
				if(*iter == clientfd)
					continue;
				bzero(send_buf, BUF_SIZE);
				memcpy(send_buf, &msg, sizeof(msg));
				if(send(*iter, send_buf, sizeof(send_buf), 0) < 0)
				{
					Log.Error(__FUNC_HEAD__, "client[%d] send message to client[%d] defaild!", clientfd, *iter);
					continue;
					// return -1;
				}
			}
		}
		else if(msg.type == 1)
		{
			bool private_offline = true;
			sprintf(format_message, SERVER_PRIVATE_MESSAGE, clientfd, msg.content);
			memcpy(msg.content, format_message, BUF_SIZE);
			// 遍历客户端列表，给目标客户端发送信息
			std::list<int>::iterator iter = clients_list.begin();
			for(; iter != clients_list.end(); ++iter)
			{
				if(*iter == msg.toID)
				{
					private_offline = false;
					bzero(send_buf, BUF_SIZE);
					memcpy(send_buf, &msg, sizeof(msg));
					if(send(*iter, send_buf, sizeof(send_buf), 0) < 0)
					{
						Log.Error(__FUNC_HEAD__, "client[%d] send private message to client[%d] defaild!", clientfd, *iter);
						return -1;
					}
				}
			}
			// 如目标不在线
			if(private_offline)
			{
				sprintf(format_message, SERVER_PRIVATE_ERROR_MESSAGE,msg.toID);
				memcpy(msg.content, format_message, BUF_SIZE);
				bzero(send_buf, BUF_SIZE);
				memcpy(send_buf, &msg, sizeof(msg));
				if(send(msg.fromID, send_buf, sizeof(send_buf), 0) < 0)
				{
					Log.Error(__FUNC_HEAD__, "return client[%d] message defaild!", msg.fromID);
					return -1;
				}
			}
		}
	}
	return len;
}

//启动服务端
void Server::StartServer()
{
	
	// struct epoll_event ev;
	// 初始化服务端
	InitServer();
	// 主循环
	// sNetMgr.Loop();
	net_thread_pool_->SendMessage("__net_work__");
	net_thread_pool_->SendMessage("__net_work__");
	net_thread_pool_->SendMessage("__net_work__");
	net_thread_pool_->SendMessage("__net_work__");
	net_thread_pool_->SendMessage("__net_work__");

	Log_Debug("gamge logic...");
	while(true);
	Log_Debug("close server!");
	// 关闭服务
	CloseServer();
}