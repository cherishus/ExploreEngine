#include "enginepch.h"
#include "SceneHierarchyPanel.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

namespace Explore
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("SceneHierarchy Panel");
		m_Context->m_Registry.each([this](entt::entity entityId) {
			Entity entity(entityId, m_Context.get());
			DrawEntityNode(entity);
		});
		ImGui::End();

		ImGui::Begin("Property Panel");
		if (m_SelectedEntity)
		{
			DrawComponentNode(m_SelectedEntity);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity& entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().TagName;
		ImGuiTabBarFlags flag = (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool bOpen = ImGui::TreeNodeEx((void*)(int32_t)entity, flag, tag.c_str());
		if (bOpen)
		{
			ImGui::TreePop();
		}
		//update select entity
		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
		}
	}

	void SceneHierarchyPanel::DrawComponentNode(Entity& entity)
	{
		//TagComponent
		auto& tag = entity.GetComponent<TagComponent>().TagName;
		char buffer[256];
		memset(buffer, 0, 256);
		memcpy_s(buffer, 256, tag.c_str(), sizeof(tag));
		if (ImGui::InputText("TagName", buffer, 256))
		{
			tag = std::string(buffer);
		}

		//TransformComponent
		auto& transform = entity.GetComponent<TransformComponent>().Transform;
		ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
	}
}