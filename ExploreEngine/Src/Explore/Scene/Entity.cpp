#include "enginepch.h"
#include "Entity.h"

namespace Explore
{
	Entity::Entity(entt::entity entity, Scene* scene)
		: m_Entity(entity), m_Scene(scene)
	{}
}