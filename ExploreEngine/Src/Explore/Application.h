#pragma once
#include "Core.h"

namespace Explore
{
	class EXPLORE_API Application
	{
	public:
		Application() {}

		virtual ~Application() {}

		void Run();
	};

	//To be defined in Application
	Application* CreateApplication();
}

