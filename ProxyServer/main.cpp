#include "stdafx.h"
#include "ProxyServer.h"

int wmain(int argc, wchar_t *argv[])
{
	ProxyServer proxy_server;
	proxy_server.StartServer();
	Sleep(INFINITE);
	return 0;
}

