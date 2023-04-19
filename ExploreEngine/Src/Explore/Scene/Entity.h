#pragma once
#include "Scene.h"
#include "entt.hpp"

namespace Explore
{
	class Entity
	{
	public:
		Entity() : m_Entity(entt::null), m_Scene(nullptr) {}

		Entity(entt::entity entity, Scene* scene);

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_Scene->m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Scene->m_Registry.get<T>(m_Entity);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_Scene->m_Registry.remove<T>(m_Entity);
		}

	private:
		entt::entity m_Entity;
		
		Scene* m_Scene;
	};
}