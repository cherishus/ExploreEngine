#include "enginepch.h"
#include "Renderer2D.h"
#include "Platform/OpenGl/OpenGLShader.h"
#include "RenderCommand.h"

namespace Explore
{
	Renderer2D::Renderer2DStorage* Renderer2D::s_Renderer2DData = nullptr;

	void Renderer2D::Init()
	{
		s_Renderer2DData = new Renderer2DStorage;

		s_Renderer2DData->QuadVertexArray = VertexArray::Create();

		{
			float vertices[] = {
				-0.5f, -0.5f, 0.0f,
				0.5f, -0.5f, 0.0f,
				0.5f, 0.5f, 0.0f,
				-0.5f,0.5f,0.0f
			};

			Ref<VertexBuffer> vertexBuffer;
			vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

			BufferLayout layout = {
				{"aPos",   ShaderDataType::Float3}
			};
			vertexBuffer->SetLayout(layout);

			s_Renderer2DData->QuadVertexArray->AddVertexBuffer(vertexBuffer);
		}

		{
			uint32_t indices[] = {
				0, 1, 2,
				2, 3, 0
			};

			Ref<IndexBuffer> indexBuffer;
			indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

			s_Renderer2DData->QuadVertexArray->SetIndexBuffer(indexBuffer);
		}

		s_Renderer2DData->FlatColorShader = Shader::Create("assets/shaders/flatcolor.glsl");
	}
	
	void Renderer2D::Shutdown()
	{
		delete s_Renderer2DData;
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		s_Renderer2DData->FlatColorShader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Renderer2DData->FlatColorShader)->UploadUnifromMat4("u_ProjectionViewMatrix",camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Renderer2DData->FlatColorShader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Renderer2DData->FlatColorShader)->UploadUnifromFloat4("u_color", color);
		std::dynamic_pointer_cast<OpenGLShader>(s_Renderer2DData->FlatColorShader)->UploadUnifromMat4("u_ModelMatrix", glm::mat4(1.0f));
		
		RenderCommand::DrawIndexd(s_Renderer2DData->QuadVertexArray);
	}
}