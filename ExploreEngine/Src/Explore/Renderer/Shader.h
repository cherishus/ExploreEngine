#pragma once
#include <string>
#include <unordered_map>
#include "Explore/Core/Core.h"
#include "glm/glm.hpp"

namespace Explore
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() = 0;

		virtual void UnBind() = 0;

		virtual std::string GetName() = 0;

		virtual void SetInt(const std::string& name, int value) = 0;

		virtual void SetIntArray(const std::string& name, int* value, uint32_t count) = 0;

		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;

		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;

		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		static Ref<Shader> Create(const std::string& filePath);
	};

	class ShaderLibrary
	{
	public:
		Ref<Shader> Load(const std::string& name, const std::string& filePath);

		Ref<Shader> Load(const std::string& filePath);
	
		void Add(const Ref<Shader>& shader);

		void Add(const std::string& name, const Ref<Shader>& shader);

		Ref<Shader> Get(const std::string& name);

		bool IsExist(const std::string& name);

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders; //key:name value:Ref<Shader>
	};
}