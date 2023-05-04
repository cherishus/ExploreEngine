#include "enginepch.h"
#include "SceneHierarchyPanel.h"
#include "imgui.h"
#include "imgui_internal.h"
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
		//SceneHierarchy Panel
		ImGui::Begin("SceneHierarchy Panel");
		m_Context->m_Registry.each([this](entt::entity entityId) {
			Entity entity(entityId, m_Context.get());
			DrawEntityNode(entity);
		});
		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) //leftmouse click in window
		{
			m_SelectedEntity = Entity();
		}
		if (ImGui::BeginPopupContextWindow(0,1,false)) //right click in blank space and pop up
		{
			if (ImGui::MenuItem("Create EmptyEntity"))
			{
				m_Context->CreateEntity("Empty Entity");
			}
			ImGui::EndPopup();
		}
		ImGui::End();

		//Property Panel
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
		flag |= ImGuiTreeNodeFlags_SpanAvailWidth; //can hover treenode in that row
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
		//rightmouse click treenode and pop up
		if (ImGui::BeginPopupContextItem(0,1))
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				m_Context->DestroyEntity(entity);
				if (m_SelectedEntity == entity)
				{
					m_SelectedEntity = Entity();
				}
			}
			ImGui::EndPopup();
		}
	}

	void SceneHierarchyPanel::DrawComponentNode(Entity& entity)
	{
		//TagComponent
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().TagName;
			char buffer[256];
			memset(buffer, 0, 256);
			memcpy_s(buffer, 256, tag.c_str(), sizeof(tag));
			if (ImGui::InputText("##TagName", buffer, 256))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		//Add button for Adding Components
		if (ImGui::Button("AddComponent"))
		{
			ImGui::OpenPopup("AddComponent");
		}
		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				m_SelectedEntity.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Sprite Rendered"))
			{
				m_SelectedEntity.AddComponent<SpriteRenderedComponent>();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();

		//TransformComponent
		DrawComponent<TransformComponent>("Transform", entity, [](TransformComponent& component) {
			DrawVec3("Position", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3("Scale", component.Scale);
		});

		//SpriteRenderedComponent
		DrawComponent<SpriteRenderedComponent>("SpriteRendered", entity, [](SpriteRenderedComponent& component) {
			ImGui::ColorEdit4("ColorEdit", glm::value_ptr(component.Color));
		});

		//CameraComponent
		DrawComponent<CameraComponent>("Camera", entity, [](CameraComponent& component) {
			auto& camera = component.Camera;
			ImGui::Checkbox("Primary", &component.bPrimary);
			ImGui::Checkbox("FixedAspectRatio", &component.bFixedAspectRatio);

			char* projectionTypeStrs[2] = { "Perspective","Orthographic" };
			char* curProjectionTypeStr = projectionTypeStrs[(int)camera.GetProjectionType()];
			if (ImGui::BeginCombo("ProjectionType", curProjectionTypeStr))
			{
				for (int i = 0; i < 2; i++)
				{
					bool bSelected = (curProjectionTypeStr == projectionTypeStrs[i]);
					if (ImGui::Selectable(projectionTypeStrs[i], bSelected))
					{
						curProjectionTypeStr = projectionTypeStrs[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (curProjectionTypeStr == projectionTypeStrs[i])
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float fov = glm::degrees(camera.GetPerspectiveFov());
				if (ImGui::DragFloat("Fov", &fov))
				{
					camera.SetPerspectiveFov(glm::radians(fov));
				}

				float nearclip = camera.GetPerspectiveNear();
				if (ImGui::DragFloat("NearClip", &nearclip))
				{
					camera.SetPerspectiveNear(nearclip);
				}

				float farclip = camera.GetPerspectiveFar();
				if (ImGui::DragFloat("FarClip", &farclip))
				{
					camera.SetPerspectiveFar(farclip);
				}
			}
			else if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float size = camera.GetOrthophicSize();
				if (ImGui::DragFloat("Size", &size))
				{
					camera.SetOrthographicSize(size);
				}

				float nearclip = camera.GetOrthographicNear();
				if (ImGui::DragFloat("NearClip", &nearclip))
				{
					camera.SetOrthographicNear(nearclip);
				}

				float farclip = camera.GetOrthographicFar();
				if (ImGui::DragFloat("FarClip", &farclip))
				{
					camera.SetOrthographicFar(farclip);
				}
			}
		});
	}

	template<typename T, typename UIFunc>
	void SceneHierarchyPanel::DrawComponent(const std::string& name, Entity entity, UIFunc function)
	{
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
			ImGuiTreeNodeFlags treeNodeFlag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
			treeNodeFlag |= ImGuiTreeNodeFlags_SpanAvailWidth;
			treeNodeFlag |= ImGuiTreeNodeFlags_Framed; //give a frame on treenode
			bool bOpen = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlag, name.c_str());
			//Add button for removing component
			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
			if (ImGui::Button("+", ImVec2{20,20}))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			ImGui::PopStyleVar();

			bool bRemove = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("RemoveComponent"))
				{
					bRemove = true;
				}
				ImGui::EndPopup();
			}

			if (bOpen)
			{
				function(component);
				ImGui::TreePop();
			}

			if (bRemove)
			{
				entity.RemoveComponent<T>();
			}
		}
	}

	void SceneHierarchyPanel::DrawVec3(const std::string& label, glm::vec3& value, float resetVal, float columnWidth)
	{
		ImGui::PushID(label.c_str());

		//first column
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0,columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		//second column
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f,0.0f });

		//x
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.15f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.15f, 0.15f, 1.0f });
		if (ImGui::Button("x",buttonSize))
		{
			value.x = resetVal;
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##x", &value.x, 0.1f);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		//y
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.15f, 0.8f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.9f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.15f, 0.8f, 0.15f, 1.0f });
		if (ImGui::Button("y", buttonSize))
		{
			value.y = resetVal;
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##y", &value.y, 0.1f);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		//z
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.15f, 0.15f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.15f, 0.15f, 0.8f, 1.0f });
		if (ImGui::Button("z", buttonSize))
		{
			value.z = resetVal;
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##z", &value.z, 0.1f);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}
}