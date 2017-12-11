#pragma once

class IEventHandler
{
public:
	IEventHandler() {};
	virtual ~IEventHandler() {};

	virtual void ProxyServerStarted(const u_short port) = 0;
	virtual void OpenConnection(const sockaddr_in sockaddr) = 0;
	virtual void CloseConnection(const sockaddr_in sockaddr) = 0;
	virtual void ClientGetData(const sockaddr_in sockaddr, const char *buffer, const size_t len) = 0;
	virtual void ClientSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len) = 0;
	virtual void ServerGetData(const sockaddr_in sockaddr, const char *buffer, const size_t len) = 0;
	virtual void ServerSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len) = 0;
};