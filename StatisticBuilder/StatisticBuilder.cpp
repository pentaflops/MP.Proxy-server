#include "stdafx.h"

#include "StatisticBuilder.hpp"

size_t StatisticBuilder::GetCountOnlineConnection()
{
	size_t count = 0;
	for (auto connection : _conecions_info)
		if (connection.second.state)
			++count;

	return count;
}

size_t StatisticBuilder::GetAllSizeOfDataSent()
{
	size_t count = 0;
	for (auto connection : _conecions_info)
		count += connection.second.size_data_sent;

	return count;
}

size_t StatisticBuilder::GetAllSizeOfDataReceived()
{
	size_t count = 0;
	for (auto connection : _conecions_info)
		count += connection.second.size_data_received;

	return count;
}

size_t StatisticBuilder::GetSizeOfDataSent(const std::string ip)
{
	auto connection = _conecions_info.find(ip);
	if (connection != _conecions_info.end())
		return connection->second.size_data_sent;
	else
		return 0;
}

size_t StatisticBuilder::GetAllSizeOfDataReceived(const std::string ip)
{
	auto connection = _conecions_info.find(ip);
	if (connection != _conecions_info.end())
		return connection->second.size_data_received;
	else
		return 0;
}

void StatisticBuilder::OpenConnection(const sockaddr_in sockaddr)
{
	std::string ip(inet_ntoa(sockaddr.sin_addr));

	_conecions_info[ip].state = true;
	_conecions_info[ip].last_time_start = time(NULL);
}

void StatisticBuilder::CloseConnection(const sockaddr_in sockaddr)
{
	std::string ip(inet_ntoa(sockaddr.sin_addr));

	_conecions_info[ip].state = false;
	if (_conecions_info[ip].last_time_start > 0)
		_conecions_info[ip].times = time(NULL) - _conecions_info[ip].last_time_start;
	else
		_conecions_info[ip].times = 0;
}

void StatisticBuilder::ClientGetData(const sockaddr_in sockaddr, const char *buffer, const size_t len)
{
	std::string ip(inet_ntoa(sockaddr.sin_addr));

	_conecions_info[ip].state = true;
	_conecions_info[ip].size_data_received += len;
}

void StatisticBuilder::ClientSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len)
{
	std::string ip(inet_ntoa(sockaddr.sin_addr));

	_conecions_info[ip].state = true;
	_conecions_info[ip].size_data_sent += len;
}