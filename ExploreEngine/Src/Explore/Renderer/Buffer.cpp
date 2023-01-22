#include "enginepch.h"
#include "Buffer.h"
#include "Platform/OpenGl/OpenGLBuffer.h"
#include "Renderer.h"
#include "Explore/Core/Log.h"

namespace Explore
{
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
			{
				EXPLORE_ASSERT(false, "RendererAPI Is None, Create VertexBuffer Failed!");
				return nullptr;
				break;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLVertexBuffer>(vertices, size);
				break;
			}
		}
		EXPLORE_ASSERT(false, "RendererAPI Is Unknown, Create VertexBuffer Failed!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
			{
				EXPLORE_ASSERT(false, "RendererAPI Is None, Create IndexBuffer Failed!");
				return nullptr;
				break;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLIndexBuffer>(indices, count);
				break;
			}
		}
		EXPLORE_ASSERT(false, "RendererAPI Is Unknown, Create IndexBuffer Failed!");
		return nullptr;
	}
}