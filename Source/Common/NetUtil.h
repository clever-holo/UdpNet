#pragma once
class NetUtil
{
public:
	NetUtil(void);
	~NetUtil(void);

	static bool WSAStartup();
	static void WSACleanup();

	static bool IsIpIllegal(const char* ipstring);

};

