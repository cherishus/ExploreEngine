#include "enginepch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGl/OpenGLTexture.h"

namespace Explore
{
	Ref<Texture> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		{
			EXPLORE_ASSERT(false, "RendererAPI Is None, Create Texture2D Failed!");
			return nullptr;
			break;
		}
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLTexture2D>(path);
			break;
		}
		}
		EXPLORE_ASSERT(false, "RendererAPI Is Unknown, Create Texture2D Failed!");
		return nullptr;
	}

	Ref<Texture> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		{
			EXPLORE_ASSERT(false, "RendererAPI Is None, Create Texture2D Failed!");
			return nullptr;
			break;
		}
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLTexture2D>(width, height);
			break;
		}
		}
		EXPLORE_ASSERT(false, "RendererAPI Is Unknown, Create Texture2D Failed!");
		return nullptr;
	}
}