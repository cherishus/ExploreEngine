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

		operator bool() { return m_Entity != entt::null; }

		operator int32_t() { return int32_t(m_Entity); }

		bool operator==(const Entity& entity) { return m_Entity == entity.m_Entity && m_Scene == entity.m_Scene; }

		bool operator!=(const Entity& entity) { return !(*this == entity); }

	private:
		entt::entity m_Entity;
		
		Scene* m_Scene;
	};
}