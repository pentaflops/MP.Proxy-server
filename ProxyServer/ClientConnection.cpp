#include "stdafx.h"
#include "ClientConnection.h"

#include <string>

int ClientConnection::GetSocketAddress(const char *buffer, size_t len, sockaddr_in &sockaddr)
{
	std::string str(buffer);
	size_t start = str.find("Host: ") + 6;
	size_t count = str.find("\r\n", start) - start;

	auto host = str.substr(start, count);

	//sockaddr_in server;
	sockaddr.sin_family = AF_INET;

	hostent *hs = gethostbyname(host.c_str());
	sockaddr.sin_addr.s_addr = *(u_long *)hs->h_addr_list[0];
	sockaddr.sin_port = htons(80);

	//Заполняем адресс из HTTP


	return 0;
}

size_t ClientConnection::GetData(char *buffer, size_t size_of_buffer)
{
	int len = recv(_socket, buffer, size_of_buffer, 0);

	if (len == SOCKET_ERROR || len == 0)
		_alive = false;

	return len;
}

void ClientConnection::SendData(const char *buffer, size_t len)
{
	int result = send(_socket, buffer, len, 0);

	if (result == SOCKET_ERROR)
		_alive = false;
}