#pragma once

#include "IConnection.hpp"

class ClientConnection : public IConnection
{
public:
	ClientConnection(SOCKET socket, sockaddr_in sock_addr) : IConnection(socket, sock_addr, true) {};
	~ClientConnection() { closesocket(_socket); }

	int GetSocketAddress(const char *buffer, size_t len, sockaddr_in &sockaddr);

	int GetData(char *buffer, int size_of_buffer, Event *_Event = nullptr);
	void SendData(const char *buffer, int len, Event *_Event = nullptr);
};
