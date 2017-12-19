#include "stdafx.h"
#include "ClientConnection.hpp"

int ClientConnection::GetSocketAddress(const char *buffer, size_t len, sockaddr_in &sockaddr)
{
	try
	{
		std::string str(buffer);
		size_t ip_start = str.find("Host: ") + 6;
		size_t ip_len = str.find("\r\n", ip_start) - ip_start;

		auto ip = str.substr(ip_start, ip_len);

		int port = 80;

		size_t delivery_port = ip.find(':');
		if (delivery_port != std::string::npos)
		{
			port = stoi(ip.substr(delivery_port + 1, ip.length()));
			ip = ip.substr(0, delivery_port);
		}

		sockaddr.sin_family = AF_INET;

		hostent *hp;
		if (isalpha(ip.c_str()[0]))
		{
			hp = gethostbyname(ip.c_str());
		}
		else
		{
			u_int addr = inet_addr(ip.c_str());
			hp = gethostbyaddr((char *)&addr, 4, AF_INET);
		}

		if (hp == NULL)
			return 1;

		memset(&sockaddr, 0, sizeof(sockaddr));
		memcpy(&(sockaddr.sin_addr), hp->h_addr, hp->h_length);
		sockaddr.sin_family = hp->h_addrtype;
		sockaddr.sin_port = htons(port);
	}
	// Ловим непредвиденные исключения и выходим с ошибкой
	catch (std::exception err)
	{
		return 1;
	}

	return 0;
}

int ClientConnection::GetData(char *buffer, int size_of_buffer, Event *_Event)
{
	int len = recv(_socket, buffer, size_of_buffer, 0);

	if (len == SOCKET_ERROR || len <= 0)
		_alive = false;
	else
	{
		if (_Event != nullptr)
			_Event->ClientGetData(_sock_addr, buffer, len);
	}

	return len;
}

void ClientConnection::SendData(const char *buffer, int len, Event *_Event)
{
	int result = send(_socket, buffer, len, 0);

	if (result == SOCKET_ERROR)
		_alive = false;
	else
	{
		if (_Event != nullptr)
			_Event->ClientSendData(_sock_addr, buffer, len);
	}
}