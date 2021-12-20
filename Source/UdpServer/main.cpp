#include <iostream>
#include "MyUdpServer.h"
#include "..\Common\NetUtil.h"

int main(int argc, char* argv[])
{

	NetUtil::WSAStartup();

	std::string strServIp;
	int			nServPort = 0;

	std::cout << "server ip: ";
	std::cin >> strServIp;

	std::cout << "server port: ";
	std::cin >> nServPort;


	MyUdpServer mySVR;
	mySVR.DoWork(strServIp.c_str(), nServPort);

	return 0;

}