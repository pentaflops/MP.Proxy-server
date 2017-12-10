#include "stdafx.h"
#include "ClientConnection.h"

// Необходимо переписать красиво, чтобы поддерживались как минимум GET и POST запросы
int ClientConnection::GetSocketAddress(const char *buffer, size_t len, sockaddr_in &sockaddr)
{
	std::string str(buffer);
	size_t start = str.find("Host: ") + 6;
	size_t count = str.find("\r\n", start) - start;

	auto host = str.substr(start, count);

	int port = 80;

	size_t delivery_port = host.find(':');
	if (delivery_port != std::string::npos)
	{
		port = stoi(host.substr(delivery_port + 1, host.length()));
		host = host.substr(0, delivery_port);
	}

	//sockaddr_in server;
	sockaddr.sin_family = AF_INET;

	hostent *hp;
	if (isalpha(host.c_str()[0])) {   /* server address is a name */
		hp = gethostbyname(host.c_str());
	}
	else  { /* Convert nnn.nnn address to a usable one */
		u_int addr = inet_addr(host.c_str());
		hp = gethostbyaddr((char *)&addr, 4, AF_INET);
	}
	if (hp != NULL)
	{
		memset(&sockaddr, 0, sizeof(sockaddr));
		memcpy(&(sockaddr.sin_addr), hp->h_addr, hp->h_length);
		//sockaddr.sin_addr.s_addr = *(u_long *)hs->h_addr;
		sockaddr.sin_family = hp->h_addrtype;
		sockaddr.sin_port = htons(port);

		//Заполняем адресс из HTTP
	}

	return 0;
}

size_t ClientConnection::GetData(char *buffer, size_t size_of_buffer, Event *_Event)
{
	int len = recv(_socket, buffer, size_of_buffer, 0);

	if (len == SOCKET_ERROR || len <= 0)
		_alive = false;

	if (_Event != nullptr)
		_Event->ClientGetData(_sock_addr, buffer, len);

	return len;
}

void ClientConnection::SendData(const char *buffer, size_t len, Event *_Event)
{
	int result = send(_socket, buffer, len, 0);

	if (result == SOCKET_ERROR)
		_alive = false;

	if (_Event != nullptr)
		_Event->ClientSendData(_sock_addr, buffer, len);
}