#pragma once
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Explore/ImGui/ImGuiLayer.h"
#include "Explore/Renderer/Shader.h"
#include "Explore/Renderer/Buffer.h"
#include "Explore/Renderer/VertexArray.h"
#include "Explore/Renderer/OrthographicCamera.h"

namespace Explore
{
	class Application
	{
	public:
		Application();

		virtual ~Application() {}

		void Run();

		void OnEvent(Event& e);
		
		void PushLayer(Layer* layer);

		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }

		inline Window& GetWindow() { return *m_Window; }
	
	private:
		bool OnWindowClose(Event& e);

		bool OnWindowResize(Event& e);

	private:
		std::unique_ptr<Window> m_Window;

		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;

		bool m_Minimize = false;

		LayerStack m_LayerStack;

		Timestep m_Timestep;

		float m_LastTime = 0.0f;

	private:
		static Application* s_Instance;
	};


	//To be defined in Application
	Application* CreateApplication();
}

