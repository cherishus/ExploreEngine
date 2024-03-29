#include "enginepch.h"
#include "Renderer.h"
#include "Explore/Core/Log.h"

namespace Explore
{
	Renderer::SceneData Renderer::s_SceneData = Renderer::SceneData();

	void Renderer::Init()
	{
		EXPLORE_PROFILE_FUNCTION()
		RenderCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		EXPLORE_PROFILE_FUNCTION()
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		EXPLORE_PROFILE_FUNCTION()
		//update SceneData
		s_SceneData.viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
		EXPLORE_PROFILE_FUNCTION()
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform)
	{
		EXPLORE_PROFILE_FUNCTION()
		shader->Bind(); //bind shader
		//update unifrom in shader
		shader->SetMat4("u_ProjectionViewMatrix",s_SceneData.viewProjectionMatrix);
		shader->SetMat4("u_ModelMatrix",transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexd(vertexArray);//bind buffer about mesh
	}
}