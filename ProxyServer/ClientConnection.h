#pragma once

#include "IConnection.h"

class ClientConnection : public IConnection
{
public:
	ClientConnection(SOCKET socket, sockaddr_in sock_addr) : IConnection(socket, sock_addr, true) {};
	~ClientConnection() { closesocket(_socket); }

	int GetSocketAddress(const char *buffer, size_t len, sockaddr_in &sockaddr);

	size_t GetData(char *buffer, size_t size_of_buffer, Event *_Event = nullptr);
	void SendData(const char *buffer, size_t len, Event *_Event = nullptr);
};