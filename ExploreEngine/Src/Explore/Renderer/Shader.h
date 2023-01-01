#pragma once
#include<string>
#include "glm/glm.hpp"

namespace Explore
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);

		~Shader();

		void Bind();

		void UnBind();

		void UploadUnifromMat4(std::string name, const glm::mat4& value);

	private:
		uint32_t m_RendererId;
	};
}