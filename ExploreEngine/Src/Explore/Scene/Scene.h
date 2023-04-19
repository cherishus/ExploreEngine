#pragma once
#include "Explore/Core/Timestep.h"
#include "entt.hpp"

namespace Explore
{
	class Entity;

	class Scene
	{
		friend class Entity;

	public:
		Scene() = default;

		~Scene() = default;

		Entity CreateEntity(const std::string& name = "Default");

		void OnUpdate(Timestep ts);

		void OnSetViewportSize(float width, float height);

	private:
		entt::registry m_Registry;
	};
}