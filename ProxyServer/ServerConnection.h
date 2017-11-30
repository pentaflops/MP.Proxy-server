#pragma once

#include "IConnection.h"

class ServerConnection : public IConnection
{
public:
	ServerConnection(sockaddr_in sock_addr) : IConnection(INVALID_SOCKET, sock_addr, false) {};
	~ServerConnection() { closesocket(_socket); }

	int Connecting();

	size_t GetData(char *buffer, size_t size_of_buffer);
	void SendData(const char *buffer, size_t len);
};