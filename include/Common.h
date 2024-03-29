// 公共头文件，包括所有需要的宏以及socket网络编程头文件，以及消息结构体（用来显示消息类别等）
// 1、定义一个函数将文件描述符fd添加到epfd表示的内科事件表中供客户端和服务端两个类使用
// 2、定义一个信息数据结构，用来表示传送的消息，结构体包括发送方fd，接收方fd，用来表示消息类别的type，还有文字信息
// 函数recv() send() write() read() 参数传递是字符串，所以在传送前/接受后要把结构体转为字符串/字符串转换为结构体
#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <memory>

#include "zzLog.h"

typedef int8_t		int8;
typedef int16_t		int16;
typedef int32_t		int32;
typedef uint8_t		uint8;
typedef uint16_t	uint16;
typedef uint32_t	uint32;
typedef int64_t		int64;
typedef uint64_t	uint64;


template <typename T> std::string toString(const T & t)
{
	std::ostringstream s;
	s << t;
	return s.str();
}


// 默认服务端IP地址
#define SERVER_IP "192.168.0.113"
// 默认服务端端口号
#define SERVER_PORT 35535

// int epoll_create(int size)中的size
// 为epoll支持的最大句柄数
#define EPOLL_SIZE 5000

// 缓冲区大小65535
#define BUF_SIZE 0xFFFF

// 新用户登录后的欢迎信息
#define SERVER_WELCOME_STR "Welcome you join to the room!"
// 其他用户收到消息的前缀
#define SERVER_MESSAGE "ClientID %d say >> %s"
#define SERVER_PRIVATE_MESSAGE "Client %d say to you privately >> %s"
#define SERVER_PRIVATE_ERROR_MESSAGE "Client %d is not in the chat room yet!"

// 退出系统
#define ROOM_EXIT "EXIT"

// 提醒你是聊天室中唯一的客户
#define CAUTION "There is only one in the chat room!"

// 注册新的fd到epollfd中
// // 参数enable_et表示是否启用ET模式，如果为true则启用，否则使用LT模式
// static void addfd(int epollfd, int fd, bool enable_et)
// {
// 	struct epoll_event ev;
// 	ev.data.fd = fd;
// 	ev.events = EPOLLIN;
// 	if(enable_et)
// 		ev.events = EPOLLIN | EPOLLET;
// 	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
// 	// 设置socket为非阻塞模式
// 	// 套接字立即放回，不管I/O是否完成，该函数所在的线程会继续运行

// 	//eg.在recv(fd...)时，该函数立即返回，在返回时，内核数据还没准备好会返回WSAEWOULDBLOCK错误代码
// 	fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK);
//     Log.Success(__FUNC_HEAD__,"fd[%d] added to epoll!\n", fd);
// }

struct Msg
{
	int type;
	int fromID;
	int toID;
	char content[BUF_SIZE];
};

#endif	// _COMMON_H_