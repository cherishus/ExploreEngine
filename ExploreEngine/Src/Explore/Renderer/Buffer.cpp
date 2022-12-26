#include "enginepch.h"
#include "Buffer.h"
#include "Platform/OpenGl/OpenGLBuffer.h"
#include "Renderer.h"
#include "Explore/Log.h"

namespace Explore
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:
			{
				EXPLORE_ASSERT(false, "RendererAPI Is None, Create VertexBuffer Failed!");
				return nullptr;
				break;
			}
			case RendererAPI::OpenGL:
			{
				return new OpenGLVertexBuffer(vertices, size);
				break;
			}
		}
		EXPLORE_ASSERT(false, "RendererAPI Is Unknown, Create VertexBuffer Failed!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:
			{
				EXPLORE_ASSERT(false, "RendererAPI Is None, Create IndexBuffer Failed!");
				return nullptr;
				break;
			}
			case RendererAPI::OpenGL:
			{
				return new OpenGLIndexBuffer(indices, count);
				break;
			}
		}
		EXPLORE_ASSERT(false, "RendererAPI Is Unknown, Create IndexBuffer Failed!");
		return nullptr;
	}
}