#include "enginepch.h"
#include "Application.h"
#include "Explore/Events/ApplicationEvent.h"
#include "Log.h"
#include "input.h"
#include "Renderer/Renderer.h"
#include "GLFW/glfw3.h"

namespace Explore
{
#define BIND_EVENT_FN(x) std::bind(&Application::##x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		EXPLORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		//m_Window->SetVSync(true);

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);
	}
	
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		EXPLORE_CORE_LOG(trace, "App Event: {0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*(--it))->OnEvent(e);
			if (e.Handled)
			{
				break;
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverLay(layer);
	}

	bool Application::OnWindowClose(Event& e)
	{
		m_Running = false;
		return true;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float currentTime = (float)glfwGetTime(); //get time by glfw, it is temporary way
			m_Timestep = currentTime - m_LastTime;
			m_LastTime = currentTime;

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(m_Timestep);
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();

			auto MousePos = Input::GetMousePosition();
			//EXPLORE_CORE_LOG(trace, "MousePos: {0},{1}", MousePos.first, MousePos.second);
		}
	}
}