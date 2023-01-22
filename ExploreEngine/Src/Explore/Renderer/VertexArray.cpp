#include "enginepch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGl/OpenGLVertexArray.h"

namespace Explore
{
	Ref<VertexArray> VertexArray::Create()
	{
		RendererAPI::API renderApi = Renderer::GetAPI();
		switch (renderApi)
		{
			case RendererAPI::API::None:
			{
				EXPLORE_ASSERT(false, "RendererAPI Is None, Create VertexArray Failed!");
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLVertexArray>();
			}
		}
		EXPLORE_ASSERT(false, "RendererAPI Is Unknown, Create VertexArray Failed!");
		return nullptr;
	}
}