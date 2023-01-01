#include "enginepch.h"
#include "Shader.h"
#include "Explore/Log.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace Explore
{
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		//vertex shader
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const GLchar* vetexSource = (const GLchar*)vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &vetexSource, nullptr);
		glCompileShader(vertexShader);

		//check for shader compile errors
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
			EXPLORE_CORE_LOG(error, "VERTEX SHADER: COMPILATION_FAILED");
		}

		//fragment shader
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* fragSource = (const GLchar*)fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &fragSource, NULL);
		glCompileShader(fragmentShader);

		//check for shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			EXPLORE_CORE_LOG(error, "FRAGMENT SHADER: COMPILATION_FAILED");
		}

		// link shaders
		unsigned int shaderProgram = glCreateProgram();
		m_RendererId = shaderProgram;

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		// check for linking errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			EXPLORE_CORE_LOG(error, "PROGRAM SHADER: LINKING_FAILED");
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererId);
	}

	void Shader::Bind()
	{
		glUseProgram(m_RendererId);
	}

	void Shader::UnBind()
	{
		glUseProgram(0);
	}

	void Shader::UploadUnifromMat4(std::string name, const glm::mat4& value)
	{
		int location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}

