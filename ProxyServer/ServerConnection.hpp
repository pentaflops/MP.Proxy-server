#pragma once

#include "IConnection.hpp"

class ServerConnection : public IConnection
{
public:
	ServerConnection(sockaddr_in sock_addr) : IConnection(INVALID_SOCKET, sock_addr, false) {};
	~ServerConnection() { closesocket(_socket); }

	int Connecting();

	int GetData(char *buffer, int size_of_buffer, Event *_Event = nullptr);
	void SendData(const char *buffer, int len, Event *_Event = nullptr);
};