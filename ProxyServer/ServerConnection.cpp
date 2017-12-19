#include "stdafx.h"
#include "ServerConnection.hpp"

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

int ServerConnection::GetData(char *buffer, int size_of_buffer, Event *_Event)
{
	int len = recv(_socket, buffer, size_of_buffer, 0);

	if (len == SOCKET_ERROR || len <= 0)
		_alive = false;

	if (_Event != nullptr)
		_Event->ServerGetData(_sock_addr, buffer, len);

	return len;
}

void ServerConnection::SendData(const char *buffer, int len, Event *_Event)
{
	int result = send(_socket, buffer, len, 0);

	if (result == SOCKET_ERROR)
		_alive = false;

	if (_Event != nullptr)
		_Event->ServerSendData(_sock_addr, buffer, len);
}