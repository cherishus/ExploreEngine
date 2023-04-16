#include "enginepch.h"
#include "Scene.h"
#include "Components.h"
#include "Explore/Renderer/Renderer2D.h"

namespace Explore
{
	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(m_Registry.create(), this);
		entity.AddComponent<TagComponent>(name);
		entity.AddComponent<TransformComponent>(glm::mat4(1.0f));
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto view = m_Registry.view<TransformComponent, SpriteComponent>();
		for (auto entity : view)
		{
			TransformComponent& transformComp = view.get<TransformComponent>(entity);
			SpriteComponent& spriteComp = view.get<SpriteComponent>(entity);
			Renderer2D::DrawQuad(transformComp.Transform, spriteComp.Color);
		}
	}
}