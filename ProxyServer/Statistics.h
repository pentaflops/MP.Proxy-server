#pragma once

#include "IEventHandler.h"

struct ConnecionInfo
{
	bool state;
	size_t size_data_sent;
	size_t size_data_received;
	size_t times;
	size_t last_time_start;
};

class Statistics : public IEventHandler
{
public:
	Statistics() {}
	~Statistics() {}

	inline size_t GetCountAllConnections() { return _conecions_info.size(); }
	size_t GetCountOnlineConnection();
	size_t GetAllSizeOfDataSent();
	size_t GetAllSizeOfDataReceived();
	size_t GetSizeOfDataSent(const std::string ip);
	size_t GetAllSizeOfDataReceived(const std::string ip);

protected:
	void ProxyServerStarted(const u_short port) {};
	void OpenConnection(const sockaddr_in sockaddr);
	void CloseConnection(const sockaddr_in sockaddr);
	void ClientGetData(const sockaddr_in sockaddr, const char *buffer, const size_t len);
	void ClientSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len);
	void ServerGetData(const sockaddr_in sockaddr, const char *buffer, const size_t len) {};
	void ServerSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len) {};

private:
	std::unordered_map<std::string, ConnecionInfo> _conecions_info;
};