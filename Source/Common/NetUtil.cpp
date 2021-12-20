#include "NetUtil.h"
#include <WS2tcpip.h>

NetUtil::NetUtil(void)
{
}


NetUtil::~NetUtil(void)
{
}

bool NetUtil::WSAStartup()
{
	WSADATA w;
	int state = ::WSAStartup(0x0202, &w);
	return (state == 0) ? true : false;
}

void NetUtil::WSACleanup()
{
	::WSACleanup();
}

bool NetUtil::IsIpIllegal(const char* ipstring)
{
	if(!ipstring || *ipstring =='\0')
		return false;

	struct sockaddr_in addr4 = {0};

	int iRet = inet_pton (AF_INET, ipstring, &addr4.sin_addr);

	return iRet == 0;

}