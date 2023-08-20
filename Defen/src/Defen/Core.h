#pragma once

#ifdef DE_PLATFORM_WINDOWS

#ifdef DE_BUILD_DLL
#define DEFEN_API __declspec(dllexport)
#else
#define DEFEN_API __declspec(dllimport)
#endif

#else
#error Defen only supports Windows

#endif

#define BIT(x) (1<<x)