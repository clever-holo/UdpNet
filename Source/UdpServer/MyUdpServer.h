#pragma once

#include <string>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

class MyUdpServer
{
public:
	MyUdpServer(void);
	~MyUdpServer(void);



public:
	bool DoWork(const char* strIp, int nPort);

private:
	SOCKET m_socket;
};

