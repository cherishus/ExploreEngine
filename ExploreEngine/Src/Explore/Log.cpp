#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Explore
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$"); //custom log format: %T->time,%n->logger name,%v->log text

		s_CoreLogger = spdlog::stdout_color_mt("Explore"); //create console logger object for explore engine and multi thread safe
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("Application");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}