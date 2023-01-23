#pragma once
#include "Explore/Renderer/Shader.h"
#include "glm/glm.hpp"

namespace Explore
{
	typedef unsigned int GLenum;

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		OpenGLShader(const std::string& filePath);

		virtual ~OpenGLShader();

		virtual void Bind() override;

		virtual void UnBind() override;

		virtual std::string GetName() override { return m_name; }

		virtual void SetInt(const std::string& name, int value) override;

		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;

		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;

		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		//--------------------------------fuctions for change unifrom-------------------------------
		void UploadUnifromInt(std::string name, int value);

		void UploadUnifromFloat(std::string name, float value);

		void UploadUnifromFloat2(std::string name, const glm::vec2& value);

		void UploadUnifromFloat3(std::string name, const glm::vec3& value);

		void UploadUnifromFloat4(std::string name, const glm::vec4& value);

		void UploadUnifromMat3(std::string name, const glm::mat3& value);

		void UploadUnifromMat4(std::string name, const glm::mat4& value);

	private:
		std::string ReadFile(const std::string& path);

		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);

		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

		std::string GetShaderNameFromPath(const std::string& path);

	private:
		uint32_t m_ShaderId; //id points to shaderProgram in GPU

		std::string m_name;
	};
}