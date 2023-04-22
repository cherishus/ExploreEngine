#pragma once
#include "SceneCamera.h"
#include "ScriptEntity.h"
#include <string>
#include "glm/glm.hpp"

namespace Explore
{
	struct TagComponent
	{
		std::string TagName;

		TagComponent() : TagName("Default") {}
		TagComponent(const std::string tagName) : TagName(tagName) {}
	};

	struct TransformComponent
	{
		glm::mat4 Transform;

		TransformComponent() : Transform(glm::mat4(1.0f)) {}
		TransformComponent(const glm::mat4& transform) : Transform(transform) {}
	};

	struct SpriteComponent
	{
		glm::vec4 Color;

		SpriteComponent() : Color({1.0f,1.0f,1.0f,1.0f}) {}
		SpriteComponent(const glm::vec4& color) : Color(color) {}
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

		template<typename T>
		void Bind()
		{
			CreateIns = []()->ScriptEntity* {return new T(); };
			DestoryIns = [](ScriptEntity* Ins) {delete Ins; Ins = nullptr; };
		}
	};
}