#pragma once

#define BUFF_SIZE 10240

class IConnection
{
public:
	IConnection(SOCKET socket, sockaddr_in sock_addr, bool alive) : _socket(socket), _sock_addr(sock_addr), _alive(alive) {};
	virtual ~IConnection() {};

	virtual size_t GetData(char *buffer, size_t size_of_buffer) = 0;
	virtual void SendData(const char *buffer, size_t len) = 0;

	inline bool isAlive() { return _alive; }

protected:
	SOCKET _socket;
	sockaddr_in _sock_addr;
	bool _alive;
};