#pragma once

#ifdef DE_PLATFORM_WINDOWS

#if DE_DYNAMIC_LINK
#ifdef DE_BUILD_DLL
#define DEFEN_API __declspec(dllexport)
#else
#define DEFEN_API __declspec(dllimport)
#endif

#else
#define DEFEN_API

#endif

#else
#error Defen only supports Windows

#endif

#ifdef DE_ENABLE_ASSERTS
#define DE_ASSERT(x, ...) {if(!(x)) {DE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#define DE_CORE_ASSERT(x, ...) { if(!(x)) { DE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

#else
#define DE_ASSERT(x, ...)
#define DE_CORE_ASSERT(x, ...)

#endif

#define BIT(x) (1<<x)

#define DE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)