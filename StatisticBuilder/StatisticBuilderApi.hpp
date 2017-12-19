#pragma once

#ifdef STATISTICBUILDER_EXPORTS
#define STATISTICBUILDER_API __declspec(dllexport)
#else
#define STATISTICBUILDER_API __declspec(dllimport)
#endif
