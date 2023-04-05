#include "enginepch.h"
#include "FrameBuffer.h"
#include "Renderer.h"
#include "Platform/OpenGl/OpenGLFrameBuffer.h"

namespace Explore
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& Spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		{
			EXPLORE_ASSERT(false, "RendererAPI Is None, Create FrameBuffer Failed!");
			return nullptr;
			break;
		}
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLFrameBuffer>(Spec);
			break;
		}
		}
		EXPLORE_ASSERT(false, "RendererAPI Is Unknown, Create FrameBuffer Failed!");
		return nullptr;
	}
}