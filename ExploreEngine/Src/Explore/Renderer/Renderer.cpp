#include "enginepch.h"
#include "Renderer.h"
#include "Explore/Log.h"

namespace Explore
{
	Renderer::SceneData Renderer::s_SceneData = Renderer::SceneData();

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		//update SceneData
		s_SceneData.viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
		
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->UploadUnifromMat4("u_Matrix",s_SceneData.viewProjectionMatrix);//update unifrom in shader
		shader->Bind(); //bind shader
		RenderCommand::DrawIndexd(vertexArray);//bind buffer about mesh
	}
}