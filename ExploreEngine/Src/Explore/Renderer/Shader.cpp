#include "enginepch.h"
#include "Shader.h"
#include "Platform/OpenGl/OpenGLShader.h"
#include "Renderer.h"

namespace Explore
{
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		{
			EXPLORE_ASSERT(false, "RendererAPI Is None, Create Shader Failed!");
			return nullptr;
			break;
		}
		case RendererAPI::API::OpenGL:
		{
			return new OpenGLShader(vertexSrc, fragmentSrc);
			break;
		}
		}
		EXPLORE_ASSERT(false, "RendererAPI Is Unknown, Create Shader Failed!");
		return nullptr;
	}
}

