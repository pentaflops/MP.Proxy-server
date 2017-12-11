#include "stdafx.h"
#include "ClientConnection.h"
#include "ServerConnection.h"
#include "ProxyServer.h"

int ProxyServer::StartServer(u_short port)
{
	// ������������� Winsock
	WSADATA wsaData;
	if (WSAStartup(0x202, &wsaData) != NO_ERROR)
	{
		WSACleanup();
		return 1;
	}

	// ������������� �������� ������ �� listen
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
	_waiting_new_connections_thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadWaitingNewConnections, (LPVOID)this, 0, 0);

	_event.ProxyServerStarted(port);

	return 0;
}

void ProxyServer::StopServer()
{
	_server_started = false;
}


// �����, ������� ��������� ��� ���������� �� ������� ����� ������-�������
DWORD ThreadWaitingNewConnections(LPVOID param)
{
	ProxyServer *proxy_server = (ProxyServer *)param;

	while (proxy_server->isServerToggle())
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

// �����, ������� ���������� ���������� ���������� ����������� �������
DWORD ThreadProcessingNewConnection(LPVOID param)
{
	ClientConnection *client_connection = ((PairPServerCConnection *)param)->client_connection;
	ProxyServer *proxy_server = ((PairPServerCConnection *)param)->proxy_server;
	delete param;

	try
	{
		if (!proxy_server->isServerToggle())
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

	// �������� ������ �� �������, ����� ������ ������ �������� (����������)
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

	// ���������� ����� ���������� ������ (��� ��������� ������� �������)
	server_connection->SendData(buffer, len);


	// ������� ��� ��������� ������ ��� �������� ������ ����� �������� � ��������
	
	// ������-������
	PairConnections client_to_server;
	client_to_server.from = client_connection;
	client_to_server.to = server_connection;
	client_to_server.proxy_server = proxy_server;
	HANDLE thread_client_to_server = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadProcessingConnectionData, (LPVOID)&client_to_server, 0, 0);
	
	// ������-������
	PairConnections server_to_client;
	server_to_client.from = server_connection;
	server_to_client.to = client_connection;
	server_to_client.proxy_server = proxy_server;
	HANDLE thread_server_to_client = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadProcessingConnectionData, (LPVOID)&server_to_client, 0, 0);

	// �������� ����������. ���� ���������� ����������� ������� � ��������� �������� ������ ������-������ ����� 2 ������� (��� ������, ���� ����� ��� �������� ������)
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

// �����, ������� ������������ �������� ������ (������-������ ��� ������-������)
DWORD ThreadProcessingConnectionData(LPVOID param)
{
	PairConnections *connections = (PairConnections *)param;
	ProxyServer *proxy_server = connections->proxy_server;

	char buffer[BUFF_SIZE];
	int len = 0;

	while (connections->from->isAlive() && connections->to->isAlive() && proxy_server->isServerToggle())
	{
		memset(buffer, 0, sizeof(buffer));

		len = connections->from->GetData(buffer, sizeof(buffer), proxy_server->GetEvent());
		if (len > 0)
			connections->to->SendData(buffer, len, proxy_server->GetEvent());
	}

	return 0;
}