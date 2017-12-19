#pragma once

#include "ProxyServerApi.hpp"

#include "IConnection.hpp"
#include "ClientConnection.hpp"
#include "Event.hpp"

DWORD ThreadWaitingNewConnections(LPVOID param);
DWORD ThreadProcessingNewConnection(LPVOID param);
DWORD ThreadProcessingConnectionData(LPVOID param);

class ProxyServer
{
public:
	PROXYSERVER_API ProxyServer() : _server_started(false), _listen_socket(INVALID_SOCKET), _waiting_new_connections_thread(NULL) {};
	PROXYSERVER_API ~ProxyServer() {
		closesocket(_listen_socket);
		TerminateThread(_waiting_new_connections_thread, 0);
		CloseHandle(_waiting_new_connections_thread);
	};

	PROXYSERVER_API int StartServer(u_short port = 80);
	PROXYSERVER_API void StopServer();

	PROXYSERVER_API inline bool isServerStarted() { return _server_started; }

	PROXYSERVER_API inline Event *GetEvent() { return &_event; }
	PROXYSERVER_API inline void SubscribeEventHandler(IEventHandler *event_handler) { _event.Subscribe(event_handler); }
	PROXYSERVER_API inline void UnsubscribeEventHandler(IEventHandler *event_handler) { _event.Unsubscribe(event_handler); }

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

enum StartProxyServerExitCode
{
	SUCCESS_STARTUP = 0,
	FAILED_WSA_STARTUP = 1,
	FAILED_CREATE_SOCKET = 2,
	FAILED_BIND_SOCKET = 3,
	FAILED_LISTEN_SOCKET = 4,
};