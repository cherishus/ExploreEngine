#pragma once
#include<string>

namespace Explore
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() = 0;

		virtual void UnBind() = 0;

		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
	};
}