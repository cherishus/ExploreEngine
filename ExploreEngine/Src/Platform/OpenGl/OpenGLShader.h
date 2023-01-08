#pragma once
#include "Explore/Renderer/Shader.h"
#include "glm/glm.hpp"

namespace Explore
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);

		virtual ~OpenGLShader();

		virtual void Bind() override;

		virtual void UnBind() override;

		//--------------------------------fuctions for change unifrom-------------------------------
		void UploadUnifromInt(std::string name, int value);

		void UploadUnifromFloat(std::string name, float value);

		void UploadUnifromFloat2(std::string name, const glm::vec2& value);

		void UploadUnifromFloat3(std::string name, const glm::vec3& value);

		void UploadUnifromFloat4(std::string name, const glm::vec4& value);

		void UploadUnifromMat3(std::string name, const glm::mat3& value);

		void UploadUnifromMat4(std::string name, const glm::mat4& value);

	private:
		uint32_t m_RendererId;
	};
}