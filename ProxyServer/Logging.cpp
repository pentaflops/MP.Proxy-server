#include "stdafx.h"
#include "Logging.h"

void Logging::ProxyServerStarted(const u_short port)
{
	printf("[LOG][PROXY SERVER STARTED][PORT: %d]\n", port);
}

void Logging::OpenConnection(const sockaddr_in sockaddr)
{
	printf("[LOG][CLIENT OPEN CONNECTION][%s:%d]\n", inet_ntoa(sockaddr.sin_addr), sockaddr.sin_port);
}

void Logging::CloseConnection(const sockaddr_in sockaddr)
{
	printf("[LOG][CLIENT CLOSE CONNECTION][%s:%d]\n", inet_ntoa(sockaddr.sin_addr), sockaddr.sin_port);
}

void Logging::ClientGetData(const sockaddr_in sockaddr, const char *buffer, const size_t len)
{
	printf("[LOG][%s][Client:GetData][LEN: %d][BUFF:]\n", inet_ntoa(sockaddr.sin_addr), len);
}

void Logging::ClientSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len)
{
	printf("[LOG][%s][Client:SendData][LEN: %d][BUFF:]\n", inet_ntoa(sockaddr.sin_addr), len);
}

void Logging::ServerGetData(const sockaddr_in sockaddr, const char *buffer, const size_t len)
{
	printf("[LOG][%s][Server:GetData][LEN: %d][BUFF:]\n", inet_ntoa(sockaddr.sin_addr), len);
}

void Logging::ServerSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len)
{
	printf("[LOG][%s][Server:SendData][LEN: %d][BUFF:]\n", inet_ntoa(sockaddr.sin_addr), len);
}