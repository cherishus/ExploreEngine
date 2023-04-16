#pragma once
#include "Entity.h"
#include "Explore/Core/Timestep.h"
#include "entt.hpp"

namespace Explore
{
	class Scene
	{
		friend class Entity;

	public:
		Scene() = default;

		~Scene() = default;

		Entity CreateEntity(const std::string& name = "Default");

		void OnUpdate(Timestep ts);

	private:
		entt::registry m_Registry;
	};
}