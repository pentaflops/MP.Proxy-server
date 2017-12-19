#include "stdafx.h"

#include "Logger.hpp"

void Logger::ProxyServerStarted(const u_short port)
{
	printf("[LOG][PROXY SERVER STARTED][PORT: %d]\n", port);
}

void Logger::OpenConnection(const sockaddr_in sockaddr)
{
	printf("[LOG][CLIENT OPEN CONNECTION][%s:%d]\n", inet_ntoa(sockaddr.sin_addr), sockaddr.sin_port);
}

void Logger::CloseConnection(const sockaddr_in sockaddr)
{
	printf("[LOG][CLIENT CLOSE CONNECTION][%s:%d]\n", inet_ntoa(sockaddr.sin_addr), sockaddr.sin_port);
}

void Logger::ClientGetData(const sockaddr_in sockaddr, const char *buffer, const size_t len)
{
	printf("[LOG][%s][Client:GetData][LEN: %d][BUFF:]\n", inet_ntoa(sockaddr.sin_addr), len);
}

void Logger::ClientSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len)
{
	printf("[LOG][%s][Client:SendData][LEN: %d][BUFF:]\n", inet_ntoa(sockaddr.sin_addr), len);
}

void Logger::ServerGetData(const sockaddr_in sockaddr, const char *buffer, const size_t len)
{
	printf("[LOG][%s][Server:GetData][LEN: %d][BUFF:]\n", inet_ntoa(sockaddr.sin_addr), len);
}

void Logger::ServerSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len)
{
	printf("[LOG][%s][Server:SendData][LEN: %d][BUFF:]\n", inet_ntoa(sockaddr.sin_addr), len);
}