#include "enginepch.h"
#include "RendererAPI.h"
#include "Platform/OpenGl/OpenGLRendererAPI.h"

namespace Explore
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	RendererAPI* RendererAPI::Create()
	{
		switch (s_API)
		{
		case API::None: return nullptr;
		case API::OpenGL: return new OpenGLRendererAPI;
		}
		return nullptr;
	}
}