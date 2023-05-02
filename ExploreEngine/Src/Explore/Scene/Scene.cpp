#include "enginepch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Explore/Renderer/Renderer2D.h"

namespace Explore
{
	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(m_Registry.create(), this);
		entity.AddComponent<TagComponent>(name);
		entity.AddComponent<TransformComponent>(glm::vec3(1.0f));
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts)
	{
		//update script
		m_Registry.view<NativeScriptComponent>().each([this,ts](entt::entity entity, NativeScriptComponent comp) {
			if (!comp.Instance)
			{
				comp.Instance = comp.CreateIns();
				comp.Instance->OnCreate(entity,this);
			}
			comp.Instance->OnUpdate(ts);
			comp.Instance->OnDestory();
			comp.DestoryIns(comp.Instance);
		});

		//get main camera
		Camera* MainCamera = nullptr;
		glm::mat4 CameraTrans;
		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view)
		{
			auto& [TransformComp, CameraComp] = view.get<TransformComponent, CameraComponent>(entity);
			if (CameraComp.bPrimary)
			{
				MainCamera = &CameraComp.Camera;
				CameraTrans = TransformComp.GetTransform();
				break;
			}
		}

		//render 2D
		if (MainCamera)
		{
			Renderer2D::BeginScene(*MainCamera, CameraTrans);

			auto view = m_Registry.view<TransformComponent, SpriteRenderedComponent>();
			for (auto entity : view)
			{
				TransformComponent& transformComp = view.get<TransformComponent>(entity);
				SpriteRenderedComponent& spriteComp = view.get<SpriteRenderedComponent>(entity);
				Renderer2D::DrawQuad(transformComp.GetTransform(), spriteComp.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnSetViewportSize(float width, float height)
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			CameraComponent& cameraComp = view.get<CameraComponent>(entity);
			if (!cameraComp.bFixedAspectRatio)
			{
				cameraComp.Camera.SetViewportSize(width, height);
			}
		}
	}
}