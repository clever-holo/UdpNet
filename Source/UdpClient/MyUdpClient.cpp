#include "MyUdpClient.h"
#include <iostream>
#include <sstream>
#include "..\Common\NetUtil.h"

MyUdpClient::MyUdpClient(void)
{
	Reset();
}

MyUdpClient::~MyUdpClient(void)
{
}

bool MyUdpClient::DoWork(const std::string& strServIp, int nServPort)
{
	Reset();

	if(!Open(strServIp, nServPort))
	{
		Reset();
		return false;
	}
		
	while(1)
	{
		// 处理发送任务
		if(!SendProc())
			break;

		// 处理接收任务
		if(!RcvProc())
			break;

		// 检查是否执行完成
		if(m_cur_status == eDone)
		{
			std::cout << "传输完成" << std::endl;
			break;
		}
		Sleep(1);
	}

	Reset();

	return true;
}


bool MyUdpClient::SendProc()
{
	bool ret = true;
	switch (m_cur_status)
	{
	case eConnect:
		// 发送连接请求
		{
			ret = SendRequest();
		}
		break;

	case eSendData:
		// 发送数据
		{
			ret = SendData();			
		}
		break;

	default:
		break;
	}

	return ret;
}

bool MyUdpClient::RcvProc()
{
	SOCKET ss2 = socket(AF_INET, SOCK_DGRAM, 0);

	fd_set fdset;
	FD_ZERO(&fdset);
	FD_SET(m_socket, &fdset);

	struct timeval fd_timeout;
	fd_timeout.tv_sec = 0;
	fd_timeout.tv_usec = 10;
	int nValidSessinCnt = select(m_socket + 1, &fdset, nullptr, nullptr, &fd_timeout);

	if (SOCKET_ERROR == nValidSessinCnt)
	{
		DWORD dwErr = GetLastError();
		std::cout << "Listen error: " << dwErr << std::endl;
		return false;
	}

	if(0 == nValidSessinCnt)
		return true;

	if (!FD_ISSET(m_socket, & fdset))
	{
		return true;
	}


	int bytes = 0;
	int addr_size = sizeof(sockaddr_in);

	char buf[UDP_PACKET_MAX_SIZE] = {0};
	
	bytes = recvfrom(m_socket, buf, UDP_PACKET_MAX_SIZE, 0, (SOCKADDR *)&m_serv_addr, &addr_size);
	if (bytes >= 0)
	{
		if(bytes > 0 )
		{
			int port = ntohs(m_serv_addr.sin_port);
			std::string strIp = inet_ntoa(m_serv_addr.sin_addr);
			std::cout << "Recive data from " << strIp << ":" << port << ", " 
					  << "Size: " << bytes << ", Content: " << buf << std::endl;

			if(m_counter == SEND_MAX_PACKET_NUM)
				m_cur_status = eDone;
			else
				m_cur_status = eSendData;
		}	
		return true;
	}
	else
	{
		return false;
	}

}


int MyUdpClient::Listen(int max_wait_ms)
{
	fd_set fdset;
	FD_ZERO(&fdset);
	FD_SET(m_socket, &fdset);
	struct timeval fd_timeout;
	fd_timeout.tv_sec = max_wait_ms / 1000;
	fd_timeout.tv_usec = max_wait_ms % 1000;
	int retval = select(m_socket + 1, &fdset, nullptr, nullptr, &fd_timeout);
	return retval;
}

bool MyUdpClient::Open(const std::string& strServIp, int nServPort)
{
	// 创建socket
	m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(INVALID_SOCKET == m_socket)
	{
		std::cout << "[ERROR] Create socket error, WSAGetLastError:[" << WSAGetLastError() << "]" << std::endl;
		return false;
	}
	
	// 创建Server地址
	if(NetUtil::IsIpIllegal(strServIp.c_str()))
	{
		std::cout << "非法的IP地址:" << strServIp << std::endl;
		return false;
	}

	memset(&m_serv_addr,0,sizeof(sockaddr_in));
	m_serv_addr.sin_family = AF_INET;
	m_serv_addr.sin_port =  htons(nServPort);
	m_serv_addr.sin_addr.s_addr = inet_addr(strServIp.c_str());


	if ( (m_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET ) {
		return false;
	}

	struct sockaddr_in addr;
	memset(&addr, 0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(5556);
	addr.sin_addr.s_addr = inet_addr("192.168.43.12");
	if (bind(m_socket, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		return false;
	}

	m_cur_status = eConnect;

	return true;
}


void MyUdpClient::Reset()
{
	CloseSocket();

	m_counter = 0;
	m_cur_status = eInit;
}

bool MyUdpClient::CloseSocket()
{
	if(INVALID_SOCKET != m_socket)
	{
		shutdown(m_socket, SD_BOTH);
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
	return true;
}

bool MyUdpClient::SendRequest()
{
	std::string str_content = "请求连接服务器...";
	if(!SendImpl(str_content.c_str(), str_content.size()))
	{
		return false;
	}
	else
	{
		m_cur_status = eWaitAck;
		return true;
	}
}

bool MyUdpClient::SendData()
{
	std::string str_content;
	int counter = m_counter + 1;

	std::stringstream s;
	s << counter;
	s >> str_content;

	if(!SendImpl(str_content.c_str(), str_content.size()))
	{
		return false;
	}
	else
	{
		m_counter++;
		m_cur_status = eWaitAck;
		return true;
	}
}

bool MyUdpClient::SendImpl(const char* pContent, unsigned int content_size)
{
	int bytes = sendto(m_socket, pContent, content_size, 0, (SOCKADDR *)&m_serv_addr,	sizeof(sockaddr_in));

	if (SOCKET_ERROR == bytes)
	{
		std::cout << "Send Error["<< WSAGetLastError() <<"], Size:" << content_size << ", Content:"<< pContent << std::endl;
		return false;
	}
	else
	{
		std::cout << "Send Success, Size:" << content_size << ", Content:"<< pContent << std::endl;
		return true;
	}
}
