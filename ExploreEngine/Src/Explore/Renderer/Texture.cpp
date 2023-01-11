#include "enginepch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGl/OpenGLTexture.h"

namespace Explore
{
	Texture* Texture2D::Create(const std::string& path)
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
			return new OpenGLTexture2D(path);
			break;
		}
		}
		EXPLORE_ASSERT(false, "RendererAPI Is Unknown, Create Texture2D Failed!");
		return nullptr;
	}
}