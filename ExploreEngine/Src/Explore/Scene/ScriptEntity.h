#pragma once
#include "Entity.h"
#include "Explore/Core/Timestep.h"

namespace Explore
{
	class ScriptEntity
	{
	public:
		virtual ~ScriptEntity() = default;

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		virtual void OnCreate(entt::entity entity, Scene* scene) { m_Entity = Entity(entity, scene); }

		virtual void OnUpdate(Timestep ts) {}

		virtual void OnDestory() {}

	protected:
		Entity m_Entity;
	};
}
