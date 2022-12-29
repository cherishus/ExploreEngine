#include "enginepch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGl/OpenGLVertexArray.h"

namespace Explore
{
	VertexArray* VertexArray::Create()
	{
		RendererAPI renderApi = Renderer::GetAPI();
		switch (renderApi)
		{
			case RendererAPI::None: 
			{
				EXPLORE_ASSERT(false, "RendererAPI Is None, Create VertexArray Failed!");
				return nullptr;
			}
			case RendererAPI::OpenGL: 
			{
				return new OpenGLVertexArray();
			}
		}
		EXPLORE_ASSERT(false, "RendererAPI Is Unknown, Create VertexArray Failed!");
		return nullptr;
	}
}