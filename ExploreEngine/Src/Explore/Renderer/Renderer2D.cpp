#include "enginepch.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Explore
{
	Renderer2D::Renderer2DStorage* Renderer2D::s_Renderer2DData = nullptr;

	void Renderer2D::Init()
	{
		EXPLORE_PROFILE_FUNCTION()
		s_Renderer2DData = new Renderer2DStorage;

		s_Renderer2DData->QuadVertexArray = VertexArray::Create();

		{
			float vertices[] = {
				-0.5f, -0.5f, 0.0f,0.0f,0.0f,
				0.5f, -0.5f, 0.0f,1.0f,0.0f,
				0.5f, 0.5f, 0.0f,1.0f,1.0f,
				-0.5f,0.5f,0.0f,0.0f,1.0f
			};

			Ref<VertexBuffer> vertexBuffer;
			vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

			BufferLayout layout = {
				{"aPos",   ShaderDataType::Float3},
				{"aTexCoor",   ShaderDataType::Float2}
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
	
		s_Renderer2DData->TextureShader = Shader::Create("assets/shaders/texture.glsl");
	
		s_Renderer2DData->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t data = 0xFFFFFFFF; //white texture: rgba = 1
		s_Renderer2DData->WhiteTexture->SetData(&data, sizeof(uint32_t));
	}
	
	void Renderer2D::Shutdown()
	{
		EXPLORE_PROFILE_FUNCTION()
		delete s_Renderer2DData;
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		EXPLORE_PROFILE_FUNCTION()
		s_Renderer2DData->TextureShader->Bind();
		s_Renderer2DData->TextureShader->SetMat4("u_ProjectionViewMatrix", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		EXPLORE_PROFILE_FUNCTION()
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x,position.y,0.0f }, rotation,size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		EXPLORE_PROFILE_FUNCTION()
		s_Renderer2DData->TextureShader->Bind();
		s_Renderer2DData->TextureShader->SetFloat4("u_color", color);
		
		//bind white texture, ensure that fragment color is euqal to u_color
		s_Renderer2DData->WhiteTexture->Bind();

		glm::mat4 transform;
		transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, {0.0f,0.0f,1.0f}) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Renderer2DData->TextureShader->SetMat4("u_ModelMatrix", transform);
		
		s_Renderer2DData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexd(s_Renderer2DData->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture)
	{
		DrawQuad({ position.x,position.y,0.0f }, rotation, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture>& texture)
	{
		EXPLORE_PROFILE_FUNCTION()
		s_Renderer2DData->TextureShader->Bind();
		texture->Bind();
		s_Renderer2DData->TextureShader->SetInt("u_Texture", 0);

		//bind white color, ensure that the fragment color is equal to u_Texture
		s_Renderer2DData->TextureShader->SetFloat4("u_color", { 1.0f,1.0f,1.0f,1.0f });

		glm::mat4 transform;
		transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f }) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Renderer2DData->TextureShader->SetMat4("u_ModelMatrix", transform);
		
		s_Renderer2DData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexd(s_Renderer2DData->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color, const Ref<Texture>& texture)
	{
		DrawQuad({ position.x,position.y,0 }, rotation, size, color, texture);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color, const Ref<Texture>& texture)
	{
		EXPLORE_PROFILE_FUNCTION()
		s_Renderer2DData->TextureShader->Bind();
		texture->Bind();
		s_Renderer2DData->TextureShader->SetInt("u_Texture", 0);

		s_Renderer2DData->TextureShader->SetFloat4("u_color", color);

		glm::mat4 transform;
		transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f }) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Renderer2DData->TextureShader->SetMat4("u_ModelMatrix", transform);

		s_Renderer2DData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexd(s_Renderer2DData->QuadVertexArray);
	}
}