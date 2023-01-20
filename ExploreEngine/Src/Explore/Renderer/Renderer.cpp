#include "enginepch.h"
#include "Renderer.h"
#include "Explore/Core/Log.h"
#include "Platform/OpenGl/OpenGLShader.h"

namespace Explore
{
	Renderer::SceneData Renderer::s_SceneData = Renderer::SceneData();

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		//update SceneData
		s_SceneData.viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
		
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform)
	{
		//update unifrom in shader
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUnifromMat4("u_ProjectionViewMatrix",s_SceneData.viewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUnifromMat4("u_ModelMatrix",transform);
		shader->Bind(); //bind shader

		RenderCommand::DrawIndexd(vertexArray);//bind buffer about mesh
	}
}