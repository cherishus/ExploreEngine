#include "enginepch.h"
#include "Renderer.h"
#include "Explore/Log.h"

namespace Explore
{
	void Renderer::BeginScene()
	{
		//EXPLORE_CORE_LOG(info, "Renderer::BeginScene()");
	}

	void Renderer::EndScene()
	{
		//EXPLORE_CORE_LOG(info, "Renderer::EndScene()");
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		//EXPLORE_CORE_LOG(info, "Renderer::Submit()");
		RenderCommand::DrawIndexd(vertexArray);
	}
}