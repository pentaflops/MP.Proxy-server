#include "stdafx.h"
#include "Logging.h"

void Logging::ProxyServerStarted(const u_short port)
{
	printf("[LOG][PROXY SERVER STARTED][PORT: %d]\n", port);
}

void Logging::OpenConnection(const sockaddr_in sockaddr)
{
	printf("[LOG][CLIENT OPEN CONNECTION][FROM: %d]\n", inet_ntoa(sockaddr.sin_addr));
}

void Logging::CloseConnection(const sockaddr_in sockaddr)
{
	printf("[LOG][CLIENT CLOSE CONNECTION][FROM: %d]\n", inet_ntoa(sockaddr.sin_addr));
}

void Logging::ClientGetData(const sockaddr_in sockaddr, const char *buffer, const size_t size_of_buffer)
{
	printf("[LOG][%s][Client:GetData][SIZE: %d][BUFF:]\n", inet_ntoa(sockaddr.sin_addr), size_of_buffer);
}

void Logging::ClientSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len)
{
	printf("[LOG][%s][Client:SendData][LEN: %d][BUFF:]\n", inet_ntoa(sockaddr.sin_addr), len);
}

void Logging::ServerGetData(const sockaddr_in sockaddr, const char *buffer, const size_t size_of_buffer)
{
	printf("[LOG][%s][Server:GetData][SIZE: %d][BUFF:]\n", inet_ntoa(sockaddr.sin_addr), size_of_buffer);
}

void Logging::ServerSendData(const sockaddr_in sockaddr, const char *buffer, const size_t len)
{
	printf("[LOG][%s][Server:SendData][LEN: %d][BUFF:]\n", inet_ntoa(sockaddr.sin_addr), len);
}