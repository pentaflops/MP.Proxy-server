#include "stdafx.h"
#include "ClientConnection.hpp"
#include "ServerConnection.hpp"
#include "ProxyServer.hpp"

int ProxyServer::StartServer(u_short port)
{
	// »нициализаци€ Winsock
	WSADATA wsaData;
	if (WSAStartup(0x202, &wsaData) != NO_ERROR)
	{
		WSACleanup();
		return FAILED_WSA_STARTUP;
	}

	// »нициализаци€ главного сокета на listen
	_listen_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (_listen_socket == INVALID_SOCKET)
	{
		WSACleanup();
		return FAILED_CREATE_SOCKET;
	}

	sockaddr_in sock_addr;

	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.s_addr = INADDR_ANY;
	sock_addr.sin_port = htons(port);

	if (bind(_listen_socket, (sockaddr *)&sock_addr, sizeof(sock_addr)) == SOCKET_ERROR)
	{
		closesocket(_listen_socket);
		WSACleanup();
		return FAILED_BIND_SOCKET;
	}

	if (listen(_listen_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(_listen_socket);
		WSACleanup();
		return FAILED_LISTEN_SOCKET;
	}

	_server_started = true;
	_waiting_new_connections_thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadWaitingNewConnections, (LPVOID)this, 0, 0);

	_event.ProxyServerStarted(port);

	return SUCCESS_STARTUP;
}

void ProxyServer::StopServer()
{
	_server_started = false;
}


// ѕоток, который принимает все соединени€ на главный сокет прокси-сервера
DWORD ThreadWaitingNewConnections(LPVOID param)
{
	ProxyServer *proxy_server = (ProxyServer *)param;

	while (proxy_server->isServerStarted())
	{
		sockaddr_in sock_addr;
		int sock_addr_len = sizeof(sock_addr);
		SOCKET client_socket = accept(proxy_server->_listen_socket, (sockaddr *)&sock_addr, &sock_addr_len);

		ClientConnection *client_connection = new ClientConnection(client_socket, sock_addr);

		proxy_server->GetEvent()->OpenConnection(sock_addr);
		PairPServerCConnection *pairPServerCConnection = new PairPServerCConnection;
		pairPServerCConnection->client_connection = client_connection;
		pairPServerCConnection->proxy_server = proxy_server;
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadProcessingNewConnection, (LPVOID)pairPServerCConnection, 0, 0);
	}

	return 0;
}

// ѕоток, который занимаетс€ обработкой отдельного подключени€ клиента
DWORD ThreadProcessingNewConnection(LPVOID param)
{
	ClientConnection *client_connection = ((PairPServerCConnection *)param)->client_connection;
	ProxyServer *proxy_server = ((PairPServerCConnection *)param)->proxy_server;
	delete param;

	try
	{
		if (!proxy_server->isServerStarted())
		{
			delete client_connection;
			return 0;
		}
	}
	catch (...)
	{
		delete client_connection;
		return 0;
	}

	sockaddr_in sockaddr;
	char buffer[BUFF_SIZE];

	// ѕолучаем данные от клиента, чтобы узнать адресс ссервера (получател€)
	int len = client_connection->GetData(buffer, sizeof(buffer));

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

	// отправл€ем ранее полученные данные (дл€ получени€ адресса сервера)
	server_connection->SendData(buffer, len);


	// —оздаем два отдельных потока дл€ передачи данных между клиентом и сервером

	// клиент-сервер
	PairConnections client_to_server;
	client_to_server.from = client_connection;
	client_to_server.to = server_connection;
	client_to_server.proxy_server = proxy_server;
	HANDLE thread_client_to_server = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadProcessingConnectionData, (LPVOID)&client_to_server, 0, 0);

	// сервер-клиент
	PairConnections server_to_client;
	server_to_client.from = server_connection;
	server_to_client.to = client_connection;
	server_to_client.proxy_server = proxy_server;
	HANDLE thread_server_to_client = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadProcessingConnectionData, (LPVOID)&server_to_client, 0, 0);

	// «акрытие соединени€. ∆дем завершени€ подключени€ клиента и завершаем передачу данных сервер-клиент через 2 секунды (или раньше, если поток сам закончил работу)
	WaitForSingleObject(thread_client_to_server, 10000);
	WaitForSingleObject(thread_server_to_client, 2000);
	TerminateThread(thread_client_to_server, 0);
	TerminateThread(thread_server_to_client, 0);
	CloseHandle(thread_client_to_server);
	CloseHandle(thread_server_to_client);

	proxy_server->GetEvent()->CloseConnection(client_connection->GetSockaddr());

	delete client_connection;
	delete server_connection;

	return 0;
}

// ѕоток, который обеспечивает передачу данных (клиент-сервер или сервер-клиент)
DWORD ThreadProcessingConnectionData(LPVOID param)
{
	PairConnections *connections = (PairConnections *)param;
	ProxyServer *proxy_server = connections->proxy_server;

	char buffer[BUFF_SIZE];
	int len = 0;

	while (connections->from->isAlive() && connections->to->isAlive() && proxy_server->isServerStarted())
	{
		memset(buffer, 0, sizeof(buffer));

		len = connections->from->GetData(buffer, sizeof(buffer), proxy_server->GetEvent());
		if (len > 0)
			connections->to->SendData(buffer, len, proxy_server->GetEvent());
	}

	return 0;
}