#pragma once

#include "IEventHandler.h"

class Event
{
public:
	Event() {};
	~Event() {};

	void Subscribe(IEventHandler *_subscriber);
	void Unsubscribe(IEventHandler *_subscriber);

	void ProxyServerStarted(const u_short port) const;
	void OpenConnection(const sockaddr_in sockaddr) const;
	void CloseConnection(const sockaddr_in sockaddr) const;
	void ClientGetData(const sockaddr_in sockaddr, const char *buffer, const size_t len) const;
	void ClientSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len) const;
	void ServerGetData(const sockaddr_in sockaddr, const char *buffer, const size_t len) const;
	void ServerSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len) const;

private:
	std::vector<IEventHandler *> _subscribers;
};