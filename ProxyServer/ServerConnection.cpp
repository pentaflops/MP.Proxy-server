#include "stdafx.h"
#include "ServerConnection.h"

int ServerConnection::Connecting()
{
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (_socket == INVALID_SOCKET)
	{
		return 1;
	}

	if (connect(_socket, (struct sockaddr*)&_sock_addr, sizeof(_sock_addr)) == SOCKET_ERROR)
	{
		return 2;
	}

	_alive = true;

	return 0;
}

size_t ServerConnection::GetData(char *buffer, size_t size_of_buffer)
{
	int len = recv(_socket, buffer, size_of_buffer, 0);

	if (len == SOCKET_ERROR || len == 0)
		_alive = false;

	printf("\t\t[Server][Get] %d\n", len);

	return len;
}

void ServerConnection::SendData(const char *buffer, size_t len)
{
	int result = send(_socket, buffer, len, 0);

	if (result == SOCKET_ERROR)
		_alive = false;

	printf("\t\t[Server][Send] %d\n", result);
}