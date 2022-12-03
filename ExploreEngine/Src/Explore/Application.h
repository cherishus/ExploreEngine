#pragma once
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"

namespace Explore
{
	class EXPLORE_API Application
	{
	public:
		Application();

		virtual ~Application() {}

		void Run();

		void OnEvent(Event& e);
		
		void PushLayer(Layer* layer);

		void PushOverlay(Layer* layer);
	
	private:
		bool OnWindowClose(Event& e);

	private:
		std::unique_ptr<Window> m_Window;

		bool m_Running = true;

		LayerStack m_LayerStack;
	};

	//To be defined in Application
	Application* CreateApplication();
}

