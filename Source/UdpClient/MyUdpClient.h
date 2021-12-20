#pragma once
#include <string>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

#define UDP_PACKET_MAX_SIZE 1024

#define SEND_MAX_PACKET_NUM 10

class MyUdpClient
{
public:
	MyUdpClient(void);
	~MyUdpClient(void);

public:
	bool DoWork(const std::string& strServIp, int nServPort);

public:
	bool SendProc();
	bool RcvProc();

protected:
	void Reset();
	bool Open(const std::string& strServIp, int nServPort);
	bool CloseSocket();

	bool SendRequest();
	bool SendData();
	bool SendImpl(const char* pContent, unsigned int content_size);

	int Listen(int max_wait_ms);

private:
	enum E_WorkStatus
	{
		eInit,
		eConnect,
		eWaitAck,
		eSendData,
		eDone,
	};

	int				m_socket;
	E_WorkStatus	m_cur_status;

	sockaddr_in      m_serv_addr;

	int m_counter;
};

