#include "stdafx.h"
#include "ProxyServer.h"
#include "Logging.h"

int wmain(int argc, wchar_t *argv[])
{
	ProxyServer proxy_server;
	Logging log;
	proxy_server.SubscribeEventHandler(&log);
	proxy_server.StartServer(8000);
	Sleep(INFINITE);
	return 0;
}

