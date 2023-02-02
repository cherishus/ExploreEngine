#include "enginepch.h"
#include "OpenGLShader.h"
#include "Explore/Core/Log.h"
#include "glm/gtc/type_ptr.hpp"
#include "glad/glad.h"

namespace Explore
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
		{
			return GL_VERTEX_SHADER;
		}
		if (type == "fragment" || type == "piexl")
		{
			return GL_FRAGMENT_SHADER;
		}
		EXPLORE_ASSERT(false, "Unknown Shader Type: {0}", type);
	}

	OpenGLShader::OpenGLShader(const std::string& name,const std::string& vertexSrc, const std::string& fragmentSrc)\
		:m_name(name)
	{
		EXPLORE_PROFILE_FUNCTION()
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaderSources);
	}

	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		EXPLORE_PROFILE_FUNCTION()
		m_name = GetShaderNameFromPath(filePath);
		std::string source = ReadFile(filePath);
		std::unordered_map<GLenum, std::string> shaderSources = PreProcess(source);
		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		EXPLORE_PROFILE_FUNCTION()
		glDeleteProgram(m_ShaderId);
	}

	std::string OpenGLShader::ReadFile(const std::string& path)
	{
		EXPLORE_PROFILE_FUNCTION()
		std::string result;
		std::ifstream in(path, std::ios::in | std::ios::binary); //in binary mode
		if (in)
		{
			in.seekg(0, std::ios::end);
			uint32_t size = (uint32_t)in.tellg(); //get size
			result.resize(size);
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size()); //read file
			in.close();
		}
		else
		{
			EXPLORE_CORE_LOG(error, "Could not open file: {0}", path);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		EXPLORE_PROFILE_FUNCTION()
		std::unordered_map<GLenum, std::string> shaderSources;
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			//get type
			size_t eol = source.find_first_of("\r\n", pos); // \n->\r\n in windows
			EXPLORE_ASSERT(eol != std::string::npos, "invalid shader source: none newline character after type");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);

			//get source
			size_t nextlinePos = source.find_first_not_of("\r\n", eol);
			EXPLORE_ASSERT(eol != std::string::npos, "invalid shader source: none nextline source after type");
			pos = source.find(typeToken, nextlinePos);
			size_t sourceLen = (pos != std::string::npos ? pos : source.length()) - nextlinePos;
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextlinePos, sourceLen);
		}
		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		EXPLORE_PROFILE_FUNCTION()
		//create shader program
		unsigned int ShaderProgram = glCreateProgram();
		m_ShaderId = ShaderProgram;

		std::vector<uint32_t> shaderIds;
		shaderIds.reserve(shaderSources.size());
		for (const auto& shaderSource : shaderSources)
		{
			//create shader
			unsigned int shaderId = glCreateShader(shaderSource.first);
			const GLchar* vetexSource = (const GLchar*)shaderSource.second.c_str();
			glShaderSource(shaderId, 1, &vetexSource, nullptr);
			glCompileShader(shaderId);

			//check for OpenGLShader compile errors
			int success;
			char infoLog[512];
			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
				EXPLORE_CORE_LOG(error, "{0} OpenGLShader: COMPILATION_FAILED", shaderSource.first);
			}

			glAttachShader(ShaderProgram, shaderId);
			shaderIds.push_back(shaderId);
		}

		// link OpenGLShaders
		glLinkProgram(ShaderProgram);

		// check for linking errors
		int success;
		char infoLog[512];
		glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
			EXPLORE_CORE_LOG(error, "PROGRAM OpenGLShader: LINKING_FAILED");
		}

		//delete shader
		for (auto shaderId : shaderIds)
		{
			glDeleteShader(shaderId);
		}
	}

	std::string OpenGLShader::GetShaderNameFromPath(const std::string& path)
	{
		EXPLORE_PROFILE_FUNCTION()
		size_t lastSlash = path.find_last_of("/\\"); //find character '/' or character '\' in path, not find string "/\\" in path
		lastSlash = lastSlash != std::string::npos ? (lastSlash + 1) : 0;
		size_t lastDot = path.rfind(".");
		size_t count = (lastDot != std::string::npos ? lastDot : path.size()) - lastSlash;
		return path.substr(lastSlash, count);
	}

	void OpenGLShader::Bind()
	{
		EXPLORE_PROFILE_FUNCTION()
		glUseProgram(m_ShaderId);
	}

	void OpenGLShader::UnBind()
	{
		EXPLORE_PROFILE_FUNCTION()
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		EXPLORE_PROFILE_FUNCTION()
		UploadUnifromInt(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		EXPLORE_PROFILE_FUNCTION()
		UploadUnifromFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		EXPLORE_PROFILE_FUNCTION()
		UploadUnifromFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		EXPLORE_PROFILE_FUNCTION()
		UploadUnifromMat4(name, value);
	}

	void OpenGLShader::UploadUnifromInt(std::string name, int value)
	{
		int location = glGetUniformLocation(m_ShaderId, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUnifromFloat(std::string name, float value)
	{
		int location = glGetUniformLocation(m_ShaderId, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUnifromFloat2(std::string name, const glm::vec2& value)
	{
		int location = glGetUniformLocation(m_ShaderId, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUnifromFloat3(std::string name, const glm::vec3& value)
	{
		int location = glGetUniformLocation(m_ShaderId, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUnifromFloat4(std::string name, const glm::vec4& value)
	{
		int location = glGetUniformLocation(m_ShaderId, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUnifromMat3(std::string name, const glm::mat3& value)
	{
		int location = glGetUniformLocation(m_ShaderId, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUnifromMat4(std::string name, const glm::mat4& value)
	{
		int	location = glGetUniformLocation(m_ShaderId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}