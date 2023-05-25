#pragma once
#include "Explore.h"
#include "Scene.h"

namespace Explore
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene)
			: m_Scene(scene)
		{}

		void Serialize(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
	
	private:
		Ref<Scene> m_Scene;
	};
}