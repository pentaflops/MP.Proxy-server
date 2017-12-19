#pragma once

#include "StatisticBuilderApi.hpp"

#include "../ProxyServer/IEventHandler.hpp"

struct ConnecionInfo
{
	bool state;
	size_t size_data_sent;
	size_t size_data_received;
	time_t times;
	time_t last_time_start;
};

class StatisticBuilder : public IEventHandler
{
public:
	STATISTICBUILDER_API StatisticBuilder() {}
	STATISTICBUILDER_API ~StatisticBuilder() {}

	STATISTICBUILDER_API inline size_t GetCountAllConnections() { return _conecions_info.size(); }
	STATISTICBUILDER_API size_t GetCountOnlineConnection();
	STATISTICBUILDER_API size_t GetAllSizeOfDataSent();
	STATISTICBUILDER_API size_t GetAllSizeOfDataReceived();
	STATISTICBUILDER_API size_t GetSizeOfDataSent(const std::string ip);
	STATISTICBUILDER_API size_t GetAllSizeOfDataReceived(const std::string ip);

protected:
	STATISTICBUILDER_API void ProxyServerStarted(const u_short port) {};
	STATISTICBUILDER_API void OpenConnection(const sockaddr_in sockaddr);
	STATISTICBUILDER_API void CloseConnection(const sockaddr_in sockaddr);
	STATISTICBUILDER_API void ClientGetData(const sockaddr_in sockaddr, const char *buffer, const size_t len);
	STATISTICBUILDER_API void ClientSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len);
	STATISTICBUILDER_API void ServerGetData(const sockaddr_in sockaddr, const char *buffer, const size_t len) {};
	STATISTICBUILDER_API void ServerSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len) {};

private:
	std::unordered_map<std::string, ConnecionInfo> _conecions_info;
};