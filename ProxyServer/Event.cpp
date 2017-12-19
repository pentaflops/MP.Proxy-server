#include "stdafx.h"
#include "Event.hpp"

void Event::Subscribe(IEventHandler *_subscriber)
{
	if (std::find(_subscribers.begin(), _subscribers.end(), _subscriber) == _subscribers.end())
		_subscribers.push_back(_subscriber);
}

void Event::Unsubscribe(IEventHandler *_subscriber)
{
	_subscribers.erase(std::remove(_subscribers.begin(), _subscribers.end(), _subscriber), _subscribers.end());
}

void Event::ProxyServerStarted(const u_short port) const
{
	for (auto &subscriber : _subscribers)
		subscriber->ProxyServerStarted(port);
}

void Event::OpenConnection(const sockaddr_in sockaddr) const
{
	for (auto &subscriber : _subscribers)
		subscriber->OpenConnection(sockaddr);
}

void Event::CloseConnection(const sockaddr_in sockaddr) const
{
	for (auto &subscriber : _subscribers)
		subscriber->CloseConnection(sockaddr);
}

void Event::ClientGetData(const sockaddr_in sockaddr, const char *buffer, const size_t size_of_buffer) const
{
	for (auto &subscriber : _subscribers)
		subscriber->ClientGetData(sockaddr, buffer, size_of_buffer);
}

void Event::ClientSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len) const
{
	for (auto &subscriber : _subscribers)
		subscriber->ClientSendData(sockaddr, buffer, len);
}

void Event::ServerGetData(const sockaddr_in sockaddr, const char *buffer, const size_t size_of_buffer) const
{
	for (auto &subscriber : _subscribers)
		subscriber->ServerGetData(sockaddr, buffer, size_of_buffer);
}

void Event::ServerSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len) const
{
	for (auto &subscriber : _subscribers)
		subscriber->ServerSendData(sockaddr, buffer, len);
}