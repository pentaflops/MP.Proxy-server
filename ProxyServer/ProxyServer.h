#pragma once

#include "IConnection.h"

DWORD ThreadWaitingNewConnections(LPVOID param);
DWORD ThreadProcessingNewConnection(LPVOID param);
DWORD ThreadProcessingConnectionData(LPVOID param);

class ProxyServer
{
public:
	ProxyServer() : _server_started(false), _listen_socket(INVALID_SOCKET) {};
	~ProxyServer() { closesocket(_listen_socket);  };

	int StartServer(u_short port = 80);
	void StopServer();

	friend DWORD ThreadWaitingNewConnections(LPVOID param);

private:
	bool _server_started;
	SOCKET _listen_socket;
};

struct PairConnections {
	IConnection *from;
	IConnection *to;
};