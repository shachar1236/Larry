#pragma once

#include <spdlog/spdlog.h>

namespace Larry {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define LA_CORE_TRACE(...)    ::Larry::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LA_CORE_INFO(...)     ::Larry::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LA_CORE_WARN(...)     ::Larry::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LA_CORE_ERROR(...)    ::Larry::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LA_CORE_FATAL(...)    ::Larry::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define LA_TRACE(...)	      ::Larry::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LA_INFO(...)	      ::Larry::Log::GetClientLogger()->info(__VA_ARGS__)
#define LA_WARN(...)	      ::Larry::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LA_ERROR(...)	      ::Larry::Log::GetClientLogger()->error(__VA_ARGS__)
#define LA_FATAL(...)	      ::Larry::Log::GetClientLogger()->fatal(__VA_ARGS__)
