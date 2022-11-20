#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"

namespace Explore {

	class EXPLORE_API Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_CoreLogger; }

		static std::shared_ptr<spdlog::logger> GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger; //for engine
		 
		static std::shared_ptr<spdlog::logger> s_ClientLogger; //for client application
	};
}

//log marcos for engine
#define EXPLORE_CORE_LOG(Level,...) \
	if(#Level == "error") ::Explore::Log::GetCoreLogger()->error(__VA_ARGS__); \
	else if(#Level == "warn") ::Explore::Log::GetCoreLogger()->warn(__VA_ARGS__); \
	else if(#Level == "info") ::Explore::Log::GetCoreLogger()->info(__VA_ARGS__); \
	else if(#Level == "trace") ::Explore::Log::GetCoreLogger()->trace(__VA_ARGS__); \

//log marcos for application
#define EXPLORE_CLIENT_LOG(Level,...) \
	if(#Level == "error") ::Explore::Log::GetClientLogger()->error(__VA_ARGS__); \
	else if(#Level == "warn") ::Explore::Log::GetClientLogger()->warn(__VA_ARGS__); \
	else if(#Level == "info") ::Explore::Log::GetClientLogger()->info(__VA_ARGS__); \
	else if(#Level == "trace") ::Explore::Log::GetClientLogger()->trace(__VA_ARGS__); \
