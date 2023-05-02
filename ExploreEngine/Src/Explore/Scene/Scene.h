#pragma once
#include "Explore/Core/Timestep.h"
#include "entt.hpp"

namespace Explore
{
	class Entity;

	/*
	*@ brief: Scene uses entt to manage all entitys, and updates itself in tick 
	*/
	class Scene
	{
		friend class Entity;
		friend class SceneHierarchyPanel;

	public:
		Scene() = default;

		~Scene() = default;

		Entity CreateEntity(const std::string& name = "Default");

		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);

		void OnSetViewportSize(float width, float height);

	private:
		entt::registry m_Registry;
	};
}