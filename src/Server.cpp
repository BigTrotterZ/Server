// Server.cpp
#include <iostream>

#include "Server.h"
//#include "zzLog.h"
// 服务端类成员函数

// 服务端类构造函数
Server::Server()
{
	// 初始化服务器地址和端口
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	// 初始化socket
	listener = 0;
	// epoll fd
	epfd = 0;
}

// 初始化服务器并启动监听
void Server::InitServer()
{
	Log.Debug(__FUNC_HEAD__, "Init Server ...");

	// 创建监听socket
	listener = socket(PF_INET, SOCK_STREAM, 0);
	if(listener < 0)
	{
		Log.Error(__FUNC_HEAD__, "listener");
		exit(-1);
	}

	// 避免重复绑定不成功
	int on = 1;
	if ((setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0)
	{
		Log.Error(__FUNC_HEAD__, "setsockopt faild!");
	}

	// 绑定地址
	if(bind(listener, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		Log.Error(__FUNC_HEAD__, "bind error");
		exit(-1);
	}

	// 监听
	int ret = listen(listener, 5);
	if(ret < 0)
	{
		Log.Error(__FUNC_HEAD__, "listen error");
		exit(-1);
	}

	Log.Notice(__FUNC_HEAD__, "Start to listen: %s:%u", SERVER_IP, SERVER_PORT);

	// 在内核中创建时间表epfd是一个句柄
	epfd = epoll_create(EPOLL_SIZE);
	if(epfd < 0)
	{
		Log.Error(__FUNC_HEAD__, "epfd error");
		exit(-1);
	}

	// 往事件表里添加监听事件
	addfd(epfd, listener, true);
}

// 关闭服务
void Server::CloseServer()
{
	// 关闭socket
	close(listener);

	// 关闭epoll监听
	close(epfd);
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
	// epoll 事件队列
	static struct epoll_event events[EPOLL_SIZE];
	// 初始化服务端
	InitServer();
	// 主循环
	while(true)
	{
		// epoll_events_count 表示就绪事件的数目
		int epoll_events_count = epoll_wait(epfd, events, EPOLL_SIZE, -1);
		if(epoll_events_count < 0)
		{
			Log.Error(__FUNC_HEAD__, "epoll failure!");
			break;
		}

		Log.Debug(__FUNC_HEAD__, "epoll_events_count[%d]", epoll_events_count);
		for(int i = 0; i < epoll_events_count; ++i)
		{
			int sockfd = events[i].data.fd;

			// 新用户连接
			if(sockfd == listener)
			{
				struct sockaddr_in client_address;
				socklen_t client_addrLength = sizeof(struct sockaddr_in);
				int clientfd = accept(listener, (struct sockaddr *)&client_address, &client_addrLength);
				Log.Notice(__FUNC_HEAD__, "client connection from: %s:%u, clientfd[%d]", 
					inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), clientfd);
				// std::cout << ntohs(client_address.sin_port) << std::endl;
				addfd(epfd, clientfd, true);

				// 服务端用list保存用户连接
				clients_list.push_back(clientfd);
				Log.Notice(__FUNC_HEAD__, "Add new clientfd[%d] to epoll", clientfd);
				Log.Notice(__FUNC_HEAD__, "Now there are [%d] clients", clients_list.size());

				// 服务端发送欢迎信息
				Log.Debug(__FUNC_HEAD__, "welcome message");
				// 向客户端发送登录成功信息
				char message[BUF_SIZE];
				bzero(message, BUF_SIZE);
				sprintf(message, SERVER_WELCOME_STR, clientfd);
				int ret = send(clientfd, message, BUF_SIZE, 0);
				if(ret < 0)
				{
					Log.Error(__FUNC_HEAD__, "send error");
					CloseServer();
					exit(-1);
				}
			}
			else 	// 处理用户发来的信息，并广播，使其他用户收到信息
			{
				int ret = SendBroadcastMessage(sockfd);
				if(ret < 0)
				{
					Log.Error(__FUNC_HEAD__, "broadcast error!");
					CloseServer();
					exit(-1);
				}
			}
		}
	}

	// 关闭服务
	CloseServer();
}