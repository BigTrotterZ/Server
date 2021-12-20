#include "NetManager.h"
#include "zzLog.h"
#include "Server.h"
#include "WorldPacket.h"
#include "ProtocolsManager.h"

void NetManager::Init()
{
	// 初始化服务器地址和端口
	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// 初始化socket
	listener = 0;
	// epoll fd
	epfd = 0;
}
void NetManager::StartListen()
{
	Log.Debug(__FUNC_HEAD__, "Init NetWork ...");

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
		Log.Error(__FUNC_HEAD__, "bind error %s", strerror(errno));
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
	AddClient(listener);
}
// int NetManager::WaitEvent()
// {
// 	return epoll_wait(epfd, events, EPOLL_SIZE, -1);
// }
void NetManager::Loop()
{
	while(true)
	{
		// epoll_events_count 表示就绪事件的数目
		Log_Debug("waiting...");
		int epoll_events_count = epoll_wait(epfd, events, EPOLL_SIZE, -1);
		// int epoll_events_count = sNetMgr.WaitEvent();
		if(epoll_events_count < 0)
		{
			Log.Error(__FUNC_HEAD__, "epoll failure!");
			break;
		}

		Log.Debug(__FUNC_HEAD__, "epoll_events_count[%d]", epoll_events_count);
		for(int i = 0; i < epoll_events_count; ++i)
		{
			int sockfd = events[i].data.fd;

			// 有新的连接
			if(sockfd == listener)
			{
				struct sockaddr_in client_address;
				socklen_t client_addrLength = sizeof(struct sockaddr_in);
				int clientfd = accept(listener, (struct sockaddr *)&client_address, &client_addrLength);
				Log.Notice(__FUNC_HEAD__, "client connection from: %s:%u, clientfd[%d]", 
					inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), clientfd);
				sNetMgr.AddClient(clientfd);

				// 服务端用list保存用户连接
				// clients_list.push_back(clientfd);
				Log.Notice(__FUNC_HEAD__, "Add new clientfd[%d] to epoll", clientfd);
				// Log.Notice(__FUNC_HEAD__, "Now there are [%d] clients", clients_list.size());

				// 服务端发送欢迎信息
				Log.Debug(__FUNC_HEAD__, "welcome message");
				// 向客户端发送登录成功信息

				// Write(connfd, message, strlen(message));
				// if (write(connfd, message, strlen(message)) != strlen(message))

				char message[BUF_SIZE];
				bzero(message, BUF_SIZE);
				snprintf(message, sizeof(message), "%s\r\n", SERVER_WELCOME_STR);
				// sprintf(message, SERVER_WELCOME_STR, clientfd);
				int ret = send(clientfd, message, BUF_SIZE, 0);
				if(ret < 0)
				{
					Log.Error(__FUNC_HEAD__, "send error");
					// CloseServer();
					exit(-1);
				}
			}
			else if(events[i].events & EPOLLIN)	// 接收到数据，读socket
			{
				Log_Debug("epollin event");
				if(sockfd < 0)
					continue;
				uint8 message[BUF_SIZE];
				bzero(message, BUF_SIZE);
				int n = recv(sockfd, message, BUF_SIZE, 0);
				if(n < 0)
				{
					if(errno == ECONNRESET)
					{
						close(sockfd);
						events[i].data.fd = -1;
					}
					else
					{
						Log_Error("recv error");
					}
				}
				else if(n == 0)
				{
					close(sockfd);
					events[i].data.fd = -1;
				}

				Log_Notice("recv : %s", message);
				WorldPacket get_packet;
				for(int i=0; i<n; ++i)
				{
					get_packet << message[i];
				}
				uint16 protoc_id;
				get_packet >> protoc_id;
				Log_Notice("packet size %u protoc_id 0x%04x", get_packet.size(), protoc_id);
				sProtocolsMgr.Exec(protoc_id, &get_packet);

				// 调用protocolsManager解析message
				
			}
			else if(events[i].events & EPOLLOUT)	// 有数据待发送
			{				
				int ret = send(sockfd, (char *)events[i].data.ptr, sizeof((char *)events[i].data.ptr), 0);
				if(ret < 0)
				{
					Log.Error(__FUNC_HEAD__, "send error");
				}

				// ev.data.fd = sockfd;
				// ev.events = EPOLLIN | EPOLLET;
				// epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
			}
			else 	
			{
				Log_Error("something error!");
			}
		}
	}
}
void NetManager::AddClient(int fd)
{
	struct epoll_event ev;
	ev.data.fd = fd;
	// ev.events = EPOLLIN;
	// if(enable_et)
		ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
	// 设置socket为非阻塞模式
	// 套接字立即放回，不管I/O是否完成，该函数所在的线程会继续运行

	//eg.在recv(fd...)时，该函数立即返回，在返回时，内核数据还没准备好会返回WSAEWOULDBLOCK错误代码
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK);
    Log.Success(__FUNC_HEAD__,"fd[%d] added to epoll!\n", fd);
}

bool NetManager::SendPacketToUser(const User & user, WorldPacket * packet)
{
	// 向客户端发送登录成功信息
	int clientfd = user.GetClientFd();
	
	struct epoll_event ev;
	ev.data.ptr = (void *)packet->contents();
	ev.data.fd = clientfd;
	ev.events = EPOLLOUT;
	epoll_ctl(epfd, EPOLL_CTL_MOD, clientfd, &ev);
	
	return true;
}