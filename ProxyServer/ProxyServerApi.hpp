#pragma once

#ifdef PROXYSERVER_EXPORTS
#define PROXYSERVER_API __declspec(dllexport)
#else
#define PROXYSERVER_API __declspec(dllimport)
#endif