#pragma once
#include "Core.h"
#include "Window.h"

namespace Explore
{
	class EXPLORE_API Application
	{
	public:
		Application();

		virtual ~Application() {}

		void Run();

		void OnEvent(Event& e);
	
	private:
		bool OnWindowClose(Event& e);

	private:
		std::unique_ptr<Window> m_Window;

		bool m_Running = true;
	};

	//To be defined in Application
	Application* CreateApplication();
}

