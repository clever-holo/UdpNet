#include <iostream>

#include "MyUdpClient.h"
#include "..\Common\NetUtil.h"

int main(int argc, char* argv[])
{
	NetUtil::WSAStartup();

	std::string strServIp;
	int			nServPort = 0;

	do 
	{
		std::cout << "server ip: ";
		std::cin >> strServIp;

		std::cout << "server port: ";
		std::cin >> nServPort;

		// 输入检查

		// 输出确认
		std::cout << std::endl << "communicating with " << strServIp << ":" << nServPort << std::endl;
		
		MyUdpClient myUdp;
		myUdp.DoWork(strServIp, nServPort);


	} while (1);


	NetUtil::WSACleanup();

	return 0;
}