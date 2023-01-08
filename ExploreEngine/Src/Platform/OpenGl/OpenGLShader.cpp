#include "enginepch.h"
#include "OpenGLShader.h"
#include "Explore/Log.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace Explore
{
	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		//vertex OpenGLShader
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const GLchar* vetexSource = (const GLchar*)vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &vetexSource, nullptr);
		glCompileShader(vertexShader);

		//check for OpenGLShader compile errors
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
			EXPLORE_CORE_LOG(error, "VERTEX OpenGLShader: COMPILATION_FAILED");
		}

		//fragment OpenGLShader
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* fragSource = (const GLchar*)fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &fragSource, NULL);
		glCompileShader(fragmentShader);

		//check for OpenGLShader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			EXPLORE_CORE_LOG(error, "FRAGMENT OpenGLShader: COMPILATION_FAILED");
		}

		// link OpenGLShaders
		unsigned int ShaderProgram = glCreateProgram();
		m_RendererId = ShaderProgram;

		glAttachShader(ShaderProgram, vertexShader);
		glAttachShader(ShaderProgram, fragmentShader);
		glLinkProgram(ShaderProgram);

		// check for linking errors
		glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
			EXPLORE_CORE_LOG(error, "PROGRAM OpenGLShader: LINKING_FAILED");
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererId);
	}

	void OpenGLShader::Bind()
	{
		glUseProgram(m_RendererId);
	}

	void OpenGLShader::UnBind()
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUnifromInt(std::string name, int value)
	{
		int location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUnifromFloat(std::string name, float value)
	{
		int location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUnifromFloat2(std::string name, const glm::vec2& value)
	{
		int location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUnifromFloat3(std::string name, const glm::vec3& value)
	{
		int location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUnifromFloat4(std::string name, const glm::vec4& value)
	{
		int location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUnifromMat3(std::string name, const glm::mat3& value)
	{
		int location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUnifromMat4(std::string name, const glm::mat4& value)
	{
		int location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}