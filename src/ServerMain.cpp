// 服务端的主函数
#include "Server.h"
#include "CommonInterfaceMain.h"

int main(int argc, char const *argv[])
{
	CommonInitInterface();
	// 创建服务端对象后启动服务端
	Server server;
	server.StartServer();
	return 0;
}