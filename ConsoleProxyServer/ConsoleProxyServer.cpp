#include "stdafx.h"

#include "../Logger/Logger.hpp"
#include "../StatisticBuilder/StatisticBuilder.hpp"
#include "../ProxyServer/ProxyServer.hpp"

DWORD WriteStatisticscLoop(LPVOID param);

int wmain(int argc, wchar_t *argv[])
{
	ProxyServer proxyServer;
	Logger logger;
	StatisticBuilder statisticBuilder;

	proxyServer.SubscribeEventHandler(&logger);
	proxyServer.SubscribeEventHandler(&statisticBuilder);
	proxyServer.StartServer(8000);

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)WriteStatisticscLoop, (LPVOID)&statisticBuilder, 0, 0);
	Sleep(INFINITE);

	return 0;
}

DWORD WriteStatisticscLoop(LPVOID param)
{
	StatisticBuilder *statisticBuilder = (StatisticBuilder *)param;

	while (true)
	{
		printf("------------------------------------------------------------------\n");
		printf("\t[ALL CONNECTIONS]\t\t|\t\t%d\n", statisticBuilder->GetCountAllConnections());
		printf("\t[ONLINE CONNECTIONS]\t\t|\t\t%d\n", statisticBuilder->GetCountOnlineConnection());
		printf("\t[ALL SIZE OF DATA SENT]\t\t|\t\t%d\n", statisticBuilder->GetAllSizeOfDataSent());
		printf("\t[ALL SIZE OF DATA RECEIVED]\t|\t\t%d\n", statisticBuilder->GetAllSizeOfDataReceived());
		printf("------------------------------------------------------------------\n");

		Sleep(5000);
	}
}