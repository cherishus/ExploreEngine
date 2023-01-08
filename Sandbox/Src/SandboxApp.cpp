#include "Explore.h"
#include "Platform/OpenGl/OpenGLShader.h"
#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class ExampleLayer : public Explore::Layer
{
public:
	ExampleLayer() : Layer("ExampleLayer"), m_Camera(-1.0f, 1.0f, -1.0f, 1.0f)
	{
		//shader
		const std::string vertexSrc = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"layout (location = 1) in vec4 aColor;\n"
			"uniform mat4 u_ProjectionViewMatrix;\n"
			"uniform mat4 u_ModelMatrix;\n"
			"out vec4 vColor;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = u_ProjectionViewMatrix * u_ModelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"   vColor = aColor;\n"
			"}\0";

		const std::string fragSrc = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"in vec4 vColor;\n"
			"uniform vec3 u_color;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(u_color,1.0f);\n"
			"}\n\0";

		m_Shader.reset(Explore::Shader::Create(vertexSrc, fragSrc));

		//bind VAO
		m_VertexArray.reset(Explore::VertexArray::Create());

		//vertex buffer
		{
			//vertices data in Normalized Device Coordinates(NDC)
			float vertices[] = {
				-0.5f, -0.5f, 0.0f, 1.0f,0.0f,0.0f,1.0f,
				0.5f, -0.5f, 0.0f,0.0f,1.0f,0.0f,1.0f,
				0.0f, 0.5f, 0.0f,0.0f,0.0f,1.0f,1.0f
			};

			//create VBO
			Explore::Ref<Explore::VertexBuffer> vertexBuffer;
			vertexBuffer.reset(Explore::VertexBuffer::Create(vertices, sizeof(vertices)));

			//buffer layout
			Explore::BufferLayout layout = {
				{"aPos",   Explore::ShaderDataType::Float3},
				{"aColor", Explore::ShaderDataType::Float4}
			};
			vertexBuffer->SetLayout(layout);

			//bind VBO to VAO
			m_VertexArray->AddVertexBuffer(vertexBuffer);
		}

		//index buffer
		{
			//index data
			uint32_t indices[] = { //index begins from 0 
				0, 1, 2, //first triangle
			};

			//create EBO
			Explore::Ref<Explore::IndexBuffer> indexBuffer;
			indexBuffer.reset(Explore::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

			//bind EBO to VAO
			m_VertexArray->SetIndexBuffer(indexBuffer);
		}
	}

	void OnUpdate(Timestep ts) override
	{
		OnUpdateCamera(ts);

		Explore::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Explore::RenderCommand::Clear();

		Explore::Renderer::BeginScene(m_Camera);
		
		//change u_color in fragmentShader
		m_Shader->Bind();
		std::dynamic_pointer_cast<Explore::OpenGLShader>(m_Shader)->UploadUnifromFloat3("u_color", m_TriColor);
		
		//change model transform
		for (int y=0;y<3;y++)
		{
			for (int x=0;x<3;x++)
			{
				glm::vec3 location = { -0.5 + 0.5 * x, 0.5 - 0.5 * y,0.0f };
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), location);
				transform = glm::scale(transform, { 0.3f,0.3f,0.3f });
				Explore::Renderer::Submit(m_Shader, m_VertexArray,transform); //provide shader and VAO containing the reference of VBO and EAO
			}
		}

		Explore::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		//change color UI
		ImGui::Begin("Setting");
		ImGui::ColorEdit3("ColorPicker", glm::value_ptr(m_TriColor));
		ImGui::End();
	}

	void OnUpdateCamera(Timestep ts)
	{
		//control camera by input polling
		if (Explore::Input::IsKeyPressed(EXPLORE_KEY_LEFT))
		{
			m_CameraLocation.x -= m_CameraMoveSpeed * ts;
		}
		if (Explore::Input::IsKeyPressed(EXPLORE_KEY_RIGHT))
		{
			m_CameraLocation.x += m_CameraMoveSpeed * ts;
		}
		if (Explore::Input::IsKeyPressed(EXPLORE_KEY_DOWN))
		{
			m_CameraLocation.y -= m_CameraMoveSpeed * ts;
		}
		if (Explore::Input::IsKeyPressed(EXPLORE_KEY_UP))
		{
			m_CameraLocation.y += m_CameraMoveSpeed * ts;
		}
		if (Explore::Input::IsKeyPressed(EXPLORE_KEY_A))
		{
			m_CameraRotation += m_CameraRotateSpeed * ts;
		}
		if (Explore::Input::IsKeyPressed(EXPLORE_KEY_D))
		{
			m_CameraRotation -= m_CameraRotateSpeed * ts;
		}

		m_Camera.SetLocation(m_CameraLocation);
		m_Camera.SetRotaion(m_CameraRotation);
	}

	void OnEvent(Explore::Event& event) override
	{
		//control camera by key event
		//Explore::EventDispatcher eventDispathcer(event);
		//eventDispathcer.Dispatch<Explore::KeyPressedEvent>(EXPLORE_BIND_EVENT_FN(ExampleLayer::OnKeyEvent));
	}

	bool OnKeyEvent(Explore::KeyPressedEvent& event)
	{
		if (event.GetKeycode() == EXPLORE_KEY_LEFT)
		{
			m_CameraLocation.x -= m_CameraMoveSpeed;
		}
		else if (event.GetKeycode() == EXPLORE_KEY_RIGHT)
		{
			m_CameraLocation.x += m_CameraMoveSpeed;
		}

		if (event.GetKeycode() == EXPLORE_KEY_DOWN)
		{
			m_CameraLocation.y -= m_CameraMoveSpeed;
		}
		else if (event.GetKeycode() == EXPLORE_KEY_UP)
		{
			m_CameraLocation.y += m_CameraMoveSpeed;
		}

		if (event.GetKeycode() == EXPLORE_KEY_A)
		{
			m_CameraRotation -= m_CameraRotateSpeed;
		}
		else if (event.GetKeycode() == EXPLORE_KEY_D)
		{
			m_CameraRotation += m_CameraRotateSpeed;
		}
		return false;
	}

private:
	Explore::Ref<Explore::Shader> m_Shader;

	Explore::Ref<Explore::VertexArray> m_VertexArray;

	Explore::OrthographicCamera m_Camera;

	glm::vec3 m_CameraLocation = { 0.0f,0.0f,0.0f };

	float m_CameraMoveSpeed = 1.0f;

	float m_CameraRotation = 0.0f;

	float m_CameraRotateSpeed = 20.0f;

	glm::vec3 m_TriColor = { 1.0f,0.0f,0.0f };
};

class Sandbox : public Explore::Application
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer);
	}

	~Sandbox() {}
};

Explore::Application* Explore::CreateApplication()
{
	return new Sandbox();
}