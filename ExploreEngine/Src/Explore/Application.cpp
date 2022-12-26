#include "enginepch.h"
#include "Application.h"
#include "Explore/Events/ApplicationEvent.h"
#include "Log.h"
#include "input.h"
#include "Renderer/Buffer.h"
#include "glad/glad.h"

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

		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);

		//shader
		const std::string vertexSrc = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"out vec3 vPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"	vPos = aPos;\n"
			"}\0";

		const std::string fragSrc = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"in vec3 vPos;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(vPos+0.5f, 1.0f);\n"
			"}\n\0";

		m_Shader.reset(new Shader(vertexSrc, fragSrc));

		//bind VAO
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		//vertices data in Normalized Device Coordinates(NDC)
		float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f,
		};
		//bind VBO
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		//index data
		uint32_t indices[] = { //index begins from 0 
			0, 1, 2, //first triangle
		};
		//bind EBO
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		//ser vertex attribute: explain GL_ARRAY_BUFFER data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //first parameter 0 represents location£¬layout/components/type/normalize/stride/offset
		glEnableVertexAttribArray(0);

		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glBindVertexArray(0);
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
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glBindVertexArray(m_VAO);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
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