#pragma once

#include "IEventHandler.h"

class Logging : public IEventHandler
{
public:
	Logging() {}
	~Logging() {}

	void ProxyServerStarted(const u_short port);
	void OpenConnection(const sockaddr_in sockaddr);
	void CloseConnection(const sockaddr_in sockaddr);
	void ClientGetData(const sockaddr_in sockaddr, const char *buffer, const size_t len);
	void ClientSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len);
	void ServerGetData(const sockaddr_in sockaddr, const char *buffer, const size_t len);
	void ServerSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len);
private:
};