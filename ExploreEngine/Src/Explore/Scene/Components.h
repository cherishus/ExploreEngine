#pragma once
#include "SceneCamera.h"
#include "ScriptEntity.h"
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Explore
{
	/*
	* @brief: components for entity
	*/
	struct TagComponent
	{
		std::string TagName = "Default Entity";

		TagComponent() = default;

		TagComponent(const std::string tagName) : TagName(tagName) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = {0.0f,0.0f,0.0f};
		glm::vec3 Rotation = {0.0f,0.0f,0.0f};
		glm::vec3 Scale = {1.0f,1.0f,1.0f};

		TransformComponent() = default;

		TransformComponent(const glm::vec3& translation) 
			: Translation(translation)
		{}

		glm::mat4 GetTransform() {
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1.0f,0.0f,0.0f })
				* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0.0f,1.0f,0.0f })
				* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0.0f,0.0f,1.0f });

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRenderedComponent
	{
		glm::vec4 Color = { 1.0f,1.0f,1.0f,1.0f };

		SpriteRenderedComponent() = default;

		SpriteRenderedComponent(const glm::vec4& color) : Color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool bPrimary = true;
		bool bFixedAspectRatio = false;

		CameraComponent() = default;
	};

	struct NativeScriptComponent
	{
		ScriptEntity* Instance;
		std::function<ScriptEntity*()> CreateIns;
		std::function<void(ScriptEntity*)> DestoryIns;

		NativeScriptComponent() = default;

		template<typename T>
		void Bind()
		{
			CreateIns = []()->ScriptEntity* {return new T(); };
			DestoryIns = [](ScriptEntity* Ins) {delete Ins; Ins = nullptr; };
		}
	};
}