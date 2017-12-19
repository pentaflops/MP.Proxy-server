#pragma once

#include "../ProxyServer/IEventHandler.hpp"
#include "LoggerApi.hpp"

class Logger : public IEventHandler
{
public:
	LOGGER_API Logger() {}
	LOGGER_API ~Logger() {}

	LOGGER_API void ProxyServerStarted(const u_short port);
	LOGGER_API void OpenConnection(const sockaddr_in sockaddr);
	LOGGER_API void CloseConnection(const sockaddr_in sockaddr);
	LOGGER_API void ClientGetData(const sockaddr_in sockaddr, const char *buffer, const size_t len);
	LOGGER_API void ClientSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len);
	LOGGER_API void ServerGetData(const sockaddr_in sockaddr, const char *buffer, const size_t len);
	LOGGER_API void ServerSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len);

private:
};
