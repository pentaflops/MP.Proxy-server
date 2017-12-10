#pragma once

#include "IConnection.h"
#include "ClientConnection.h"
#include "Event.h"

DWORD ThreadWaitingNewConnections(LPVOID param);
DWORD ThreadProcessingNewConnection(LPVOID param);
DWORD ThreadProcessingConnectionData(LPVOID param);

class ProxyServer
{
public:
	ProxyServer() : _server_started(false), _listen_socket(INVALID_SOCKET), _waiting_new_connections_thread(NULL) {};
	~ProxyServer() {
		closesocket(_listen_socket);
		TerminateThread(_waiting_new_connections_thread, 0);
		CloseHandle(_waiting_new_connections_thread);
	};

	int StartServer(u_short port = 80);
	void StopServer();

	inline bool isServerToggle() { return _server_started; }

	inline Event *GetEvent() { return &_event; }
	inline void SubscribeEventHandler(IEventHandler *event_handler) { _event.Subscribe(event_handler); }
	inline void UnsubscribeEventHandler(IEventHandler *event_handler) { _event.Unsubscribe(event_handler); }

	friend DWORD ThreadWaitingNewConnections(LPVOID param);

private:
	bool _server_started;
	SOCKET _listen_socket;
	HANDLE _waiting_new_connections_thread;
	Event _event;
};

struct PairPServerCConnection {
	ClientConnection *client_connection;
	ProxyServer *proxy_server;
};

struct PairConnections {
	IConnection *from;
	IConnection *to;
	ProxyServer *proxy_server;
};