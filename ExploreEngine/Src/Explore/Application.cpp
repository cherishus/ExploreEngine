#include "enginepch.h"
#include "Application.h"
#include "Explore/Events/ApplicationEvent.h"
#include "Log.h"

namespace Explore
{
	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		EXPLORE_CORE_LOG(info, e);

		while (true);
	}
}