#include "enginepch.h"
#include "Shader.h"
#include "Platform/OpenGl/OpenGLShader.h"
#include "Renderer.h"

namespace Explore
{
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
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
				return std::make_shared<OpenGLShader>(name,vertexSrc, fragmentSrc);
				break;
			}
		}
		EXPLORE_ASSERT(false, "RendererAPI Is Unknown, Create Shader Failed!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& filePath)
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
				return std::make_shared<OpenGLShader>(filePath);
				break;
			}
		}
		EXPLORE_ASSERT(false, "RendererAPI Is Unknown, Create Shader Failed!");
		return nullptr;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath) //first parameter: custom name in shaderLibrary
	{
		Ref<Shader> shader = Shader::Create(filePath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filePath)
	{
		Ref<Shader> shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		std::string name = shader->GetName();
		EXPLORE_ASSERT(!IsExist(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) //first parameter: custom name in shaderLibrary
	{
		EXPLORE_ASSERT(!IsExist(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		Ref<Shader> shader;
		if (IsExist(name))
		{
			shader = m_Shaders[name];
		}
		else
		{
			EXPLORE_CORE_LOG(error, "shader not exists, name is {0}", name);
		}
		return shader;
	}

	bool ShaderLibrary::IsExist(const std::string& name)
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}

