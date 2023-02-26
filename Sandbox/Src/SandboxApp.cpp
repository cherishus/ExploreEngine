#include "Explore.h"
#include "Explore/Core/Entrypoint.h"
#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Sandbox2D.h"
#include "2DGame/GameLayer.h"

class ExampleLayer : public Explore::Layer
{
public:
	ExampleLayer() : Layer("ExampleLayer"),m_CameraController(1.7f,true)
	{
		//bind VAO
		m_VertexArray = Explore::VertexArray::Create();

		//vertex buffer
		{
			//vertices data in Normalized Device Coordinates(NDC)
			float vertices[] = {
				-0.5f, -0.5f, 0.0f, 0.0f,0.0f,
				0.5f, -0.5f, 0.0f,1.0f,0.0f,
				0.5f, 0.5f, 0.0f,1.0f,1.0f,
				-0.5f,0.5f,0.0f,0.0f,1.0f
			};

			//create VBO
			Explore::Ref<Explore::VertexBuffer> vertexBuffer;
			vertexBuffer = Explore::VertexBuffer::Create(vertices, sizeof(vertices));

			//buffer layout
			Explore::BufferLayout layout = {
				{"aPos",   Explore::ShaderDataType::Float3},
				{"aTexCoor", Explore::ShaderDataType::Float2}
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
				2, 3, 0 //second triangle
			};

			//create EBO
			Explore::Ref<Explore::IndexBuffer> indexBuffer;
			indexBuffer = Explore::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

			//bind EBO to VAO
			m_VertexArray->SetIndexBuffer(indexBuffer);
		}

		//shader
		const std::string vertexSrc = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"layout (location = 1) in vec2 aTexCoor;\n"
			"uniform mat4 u_ProjectionViewMatrix;\n"
			"uniform mat4 u_ModelMatrix;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = u_ProjectionViewMatrix * u_ModelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";

		const std::string fragSrc = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"uniform vec3 u_color;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(u_color,1.0f);\n"
			"}\0";

		Explore::Ref<Explore::Shader> colorShader = Explore::Shader::Create("color",vertexSrc, fragSrc);
		m_ShaderLibraray.Add(colorShader);

		Explore::Ref<Explore::Shader> textureShader = m_ShaderLibraray.Load("assets/shaders/texture.glsl");

		std::string path = "assets/textures/basketball.jpg";
		m_Texture = Explore::Texture2D::Create(path);

		std::string path2 = "assets/textures/grass.png";
		m_AlphaTexture = Explore::Texture2D::Create(path2);
	}

	void OnUpdate(Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);

		Explore::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Explore::RenderCommand::Clear();

		Explore::Renderer::BeginScene(m_CameraController.GetCamera());
		
		//change u_color in fragmentShader
		auto& colorShader = m_ShaderLibraray.Get("color");
		colorShader->Bind();
		colorShader->SetFloat3("u_color", m_TriColor);
		
		//change model transform
		for (int y=0;y<3;y++)
		{
			for (int x=0;x<3;x++)
			{
				glm::vec3 location = { -0.5 + 0.5 * x, 0.5 - 0.5 * y,0.0f };
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), location);
				transform = glm::scale(transform, { 0.3f,0.3f,0.3f });
				Explore::Renderer::Submit(colorShader, m_VertexArray,transform); //provide shader and VAO containing the reference of VBO and EAO
			}
		}

		auto& textureShader = m_ShaderLibraray.Get("texture");
		m_Texture->Bind();
		textureShader->Bind();
		textureShader->SetInt("u_Texture", 0);
		Explore::Renderer::Submit(textureShader, m_VertexArray,glm::mat4(1.0f));

		m_AlphaTexture->Bind();
		Explore::Renderer::Submit(textureShader, m_VertexArray, glm::mat4(1.0f));

		Explore::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		//change color UI
		ImGui::Begin("Setting");
		ImGui::ColorEdit3("ColorPicker", glm::value_ptr(m_TriColor));
		ImGui::End();
	}

	void OnEvent(Explore::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}

private:
	Explore::ShaderLibrary m_ShaderLibraray;

	Explore::Ref<Explore::VertexArray> m_VertexArray;

	Explore::OrthographicCameraController m_CameraController;

	glm::vec3 m_TriColor = { 1.0f,0.0f,0.0f };

	Explore::Ref<Explore::Texture> m_Texture,m_AlphaTexture;
};

class Sandbox : public Explore::Application
{
public:
	Sandbox() 
	{
		//PushLayer(new ExampleLayer);
		//PushLayer(new Sandbox2D);
		PushLayer(new GameLayer);
	}

	~Sandbox() {}
};

Explore::Application* Explore::CreateApplication()
{
	return new Sandbox();
}