#pragma once

#include "ProxyServerApi.hpp"

class Event;
struct sockaddr_in;

class IEventHandler
{
public:
	PROXYSERVER_API IEventHandler() {};
	PROXYSERVER_API virtual ~IEventHandler() {};

	friend Event;

protected:
	PROXYSERVER_API virtual void ProxyServerStarted(const u_short port) = 0;
	PROXYSERVER_API virtual void OpenConnection(const sockaddr_in sockaddr) = 0;
	PROXYSERVER_API virtual void CloseConnection(const sockaddr_in sockaddr) = 0;
	PROXYSERVER_API virtual void ClientGetData(const sockaddr_in sockaddr, const char *buffer, const size_t len) = 0;
	PROXYSERVER_API virtual void ClientSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len) = 0;
	PROXYSERVER_API virtual void ServerGetData(const sockaddr_in sockaddr, const char *buffer, const size_t len) = 0;
	PROXYSERVER_API virtual void ServerSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len) = 0;
};