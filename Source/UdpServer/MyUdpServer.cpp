#include "MyUdpServer.h"
#include <iostream>

#define MAXLINE 1024

MyUdpServer::MyUdpServer(void)
{
	m_socket = INVALID_SOCKET;
}


MyUdpServer::~MyUdpServer(void)
{
}

bool MyUdpServer::DoWork(const char* strIp, int nPort)
{

	// Creating socket file descriptor
	if ( (m_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET ) {
		return false;
	}

	struct sockaddr_in addr;
	memset(&addr, 0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPort);
	addr.sin_addr.s_addr = inet_addr(strIp);
	if (bind(m_socket, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		return false;
	}

	do 
	{
		//  ’œ˚œ¢
		fd_set fdset;
		FD_ZERO(&fdset);
		FD_SET(m_socket, &fdset);
		struct timeval fd_timeout;
		fd_timeout.tv_sec = 1;
		fd_timeout.tv_usec = 10;
		int nValidSessinCnt = select(m_socket + 1, &fdset, nullptr, nullptr, &fd_timeout);

		if (SOCKET_ERROR == nValidSessinCnt)
		{
			DWORD dwErr = GetLastError();
			std::cout << "Listen error: " << dwErr << std::endl;
			return false;
		}

		if(nValidSessinCnt > 0)
		{

			if (FD_ISSET(m_socket, & fdset))
			{
				int bytes = 0;
				int addr_size = sizeof(sockaddr_in);

				char buf[MAXLINE] = {0};

				struct sockaddr_in cliaddr;
				memset(&cliaddr, 0, sizeof(cliaddr));
				bytes = recvfrom(m_socket, buf, MAXLINE, 0, (SOCKADDR *)&cliaddr, &addr_size);
				if (bytes >= 0)
				{
					if(bytes > 0 )
					{
						int port = ntohs(cliaddr.sin_port);
						std::string strIp = inet_ntoa(cliaddr.sin_addr);
						std::cout << "Recive data from " << strIp << ":" << port << ", " 
							<< "Size: " << bytes << ", Content: " << buf << std::endl;

						 int sendytes = sendto(m_socket, buf, bytes, 0, (SOCKADDR *)&cliaddr,sizeof(sockaddr_in));
						if (SOCKET_ERROR == bytes)
						{
							std::cout << "Send Error, Size:" << sendytes << ", Content:"<< buf << std::endl;
						}
						else
						{
							std::cout << "Send Success, Size:" << sendytes << ", Content:"<< buf << std::endl;
						}
					}	
				}
			}

		}

		Sleep(1);

	} while (1);
 

	return true;
}
