#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Defen
{
	class DEFEN_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()
		{
			return s_CoreLogger;
		}
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger()
		{
			return s_ClientLogger;
		}
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define DE_CORE_TRACE(...)  ::Defen::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DE_CORE_INFO(...)   ::Defen::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DE_CORE_WARN(...)   ::Defen::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DE_CORE_ERROR(...)  ::Defen::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DE_CORE_FATAL(...)  ::Defen::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define DE_TRACE(...)  ::Defen::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DE_INFO(...)   ::Defen::Log::GetClientLogger()->info(__VA_ARGS__)
#define DE_WARN(...)   ::Defen::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DE_ERROR(...)  ::Defen::Log::GetClientLogger()->error(__VA_ARGS__)
#define DE_FATAL(...)  ::Defen::Log::GetClientLogger()->fatal(__VA_ARGS__)