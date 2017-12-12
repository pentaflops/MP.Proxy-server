#include "stdafx.h"
#include "ProxyServer.h"
#include "Logging.h"
#include "Statistics.h"

DWORD WriteStatisticscLoop(LPVOID param);

int wmain(int argc, wchar_t *argv[])
{
	ProxyServer proxy_server;
	Logging log;
	Statistics statistics;
	proxy_server.SubscribeEventHandler(&log);
	proxy_server.SubscribeEventHandler(&statistics);
	proxy_server.StartServer(8000);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)WriteStatisticscLoop, (LPVOID)&statistics, 0, 0);
	Sleep(INFINITE);
	return 0;
}


DWORD WriteStatisticscLoop(LPVOID param)
{
	Statistics *statistics = (Statistics *)param;

	while (true)
	{
		printf("------------------------------------------------------------------\n");
		printf("\t[ALL CONNECTIONS]\t\t|\t\t%d\n", statistics->GetCountAllConnections());
		printf("\t[ONLINE CONNECTIONS]\t\t|\t\t%d\n", statistics->GetCountOnlineConnection());
		printf("\t[ALL SIZE OF DATA SENT]\t\t|\t\t%d\n", statistics->GetAllSizeOfDataSent());
		printf("\t[ALL SIZE OF DATA RECEIVED]\t|\t\t%d\n", statistics->GetAllSizeOfDataReceived());
		printf("------------------------------------------------------------------\n");
		Sleep(5000);
	}
}