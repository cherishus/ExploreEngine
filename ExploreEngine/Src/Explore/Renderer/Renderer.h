#pragma once
#include "RenderCommand.h"

namespace Explore
{
	//Provide Rendering Functions for Renderer
	//its level is high to RenderCommand
	class Renderer
	{
	public:
		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void BeginScene();

		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
	};
}