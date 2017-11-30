#include "stdafx.h"
#include "ProxyServer.h"
#include "ClientConnection.h"
#include "ServerConnection.h"

int ProxyServer::StartServer(u_short port)
{
	WSADATA wsaData;
	if (WSAStartup(0x202, &wsaData) != NO_ERROR)
	{
		WSACleanup();
		return 1;
	}

	_listen_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (_listen_socket == INVALID_SOCKET)
	{
		WSACleanup();
		return 2;
	}

	sockaddr_in sock_addr;

	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.s_addr = INADDR_ANY;
	sock_addr.sin_port = htons(port);

	if (bind(_listen_socket, (sockaddr *)&sock_addr, sizeof(sock_addr)) == SOCKET_ERROR)
	{
		closesocket(_listen_socket);
		WSACleanup();
		return 3;
	}

	if (listen(_listen_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(_listen_socket);
		WSACleanup();
		return 5;
	}

	_server_started = true;
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadWaitingNewConnections, (LPVOID)this, 0, 0);

	return 0;
}

void ProxyServer::StopServer()
{
	_server_started = false;
}


DWORD ThreadWaitingNewConnections(LPVOID param)
{
	ProxyServer *proxy_server = (ProxyServer *)param;

	while (proxy_server->_server_started)
	{
		sockaddr_in sock_addr;
		int sock_addr_len = sizeof(sock_addr);
		SOCKET client_socket = accept(proxy_server->_listen_socket, (sockaddr *)&sock_addr, &sock_addr_len);

		ClientConnection *client_connection = new ClientConnection(client_socket, sock_addr);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadProcessingNewConnection, (LPVOID)client_connection, 0, 0);
	}

	return 0;
}

DWORD ThreadProcessingNewConnection(LPVOID param)
{
	printf("[OPEN CONNECTION]\n");
	ClientConnection *client_connection = (ClientConnection *)param;

	sockaddr_in sockaddr;
	char buffer[BUFF_SIZE];
	size_t len = client_connection->GetData(buffer, sizeof(buffer));

	if (client_connection->GetSocketAddress(buffer, len, sockaddr) != 0)
	{
		delete client_connection;

		return 0;
	}

	ServerConnection *server_connection = new ServerConnection(sockaddr);
	if (server_connection->Connecting() != 0)
	{
		delete client_connection;
		delete server_connection;

		return 0;
	}

	server_connection->SendData(buffer, len);

	PairConnections client_to_server;
	client_to_server.from = client_connection;
	client_to_server.to = server_connection;
	HANDLE thread_client_to_server = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadProcessingConnectionData, (LPVOID)&client_to_server, 0, 0);

	PairConnections server_to_client;
	server_to_client.from = server_connection;
	server_to_client.to = client_connection;
	HANDLE thread_server_to_client = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadProcessingConnectionData, (LPVOID)&server_to_client, 0, 0);

	WaitForSingleObject(thread_client_to_server, 10000);
	WaitForSingleObject(thread_server_to_client, 10000);
	TerminateThread(thread_client_to_server, 0);
	TerminateThread(thread_server_to_client, 0);
	CloseHandle(thread_client_to_server);
	CloseHandle(thread_server_to_client);

	delete client_connection;
	delete server_connection;

	printf("[CLOSE CONNECTION]\n");

	return 0;
}

DWORD ThreadProcessingConnectionData(LPVOID param)
{
	PairConnections *connections = (PairConnections *)param;

	char buffer[BUFF_SIZE];
	int len = 0;

	while (connections->from->isAlive() && connections->to->isAlive())
	{
		memset(buffer, 0, sizeof(buffer));

		len = connections->from->GetData(buffer, sizeof(buffer));
		if (len > 0)
			connections->to->SendData(buffer, len);
	}

	return 0;
}