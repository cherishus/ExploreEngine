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
			if (ImGui::InputText("TagName", buffer, 256))
			{
				tag = std::string(buffer);
			}
		}

		ImGuiTreeNodeFlags treeNodeFlag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
		auto AddButtonForRemovingComp = [](bool& bRemove)
		{
			//Add button for removing component
			bRemove = false;
			ImGui::SameLine();
			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("RemoveComponent");
			}
			if (ImGui::BeginPopup("RemoveComponent"))
			{
				if (ImGui::MenuItem("RemoveComponent"))
				{
					bRemove = true;
				}
				ImGui::EndPopup();
			}
		};

		//TransformComponent
		if (entity.HasComponent<TransformComponent>())
		{
			bool bOpen = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlag, "Transform");
			bool bRemove = false;
			AddButtonForRemovingComp(bRemove);
			
			if (bOpen)
			{
				auto& transComp = entity.GetComponent<TransformComponent>();
				DrawVec3("Position", transComp.Translation);
				DrawVec3("Rotation", transComp.Rotation);
				DrawVec3("Scale", transComp.Scale);

				ImGui::TreePop();
			}

			if (bRemove)
			{
				entity.RemoveComponent<TransformComponent>();
			}
		}

		//SpriteRenderedComponent
		if (entity.HasComponent<SpriteRenderedComponent>())
		{
			bool bOpen = ImGui::TreeNodeEx((void*)typeid(SpriteRenderedComponent).hash_code(), treeNodeFlag, "SpriteRendered");
			bool bRemove = false;
			AddButtonForRemovingComp(bRemove);

			if (bOpen)
			{
				auto& color = entity.GetComponent<SpriteRenderedComponent>().Color;
				ImGui::ColorEdit4("ColorEdit", glm::value_ptr(color));
				
				ImGui::TreePop();
			}

			if (bRemove)
			{
				entity.RemoveComponent<SpriteRenderedComponent>();
			}
		}

		//CameraComponent
		if (entity.HasComponent<CameraComponent>())
		{
			bool bOpen = ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlag, "Camera");
			bool bRemove = false;
			AddButtonForRemovingComp(bRemove);

			if (bOpen)
			{
				auto& cameraComp = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComp.Camera;
				ImGui::Checkbox("Primary", &cameraComp.bPrimary);
				ImGui::Checkbox("FixedAspectRatio", &cameraComp.bFixedAspectRatio);

				char* projectionTypeStrs[2] = { "Perspective","Orthographic" };
				char* curProjectionTypeStr = projectionTypeStrs[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("ProjectionType",curProjectionTypeStr))
				{
					for (int i = 0; i < 2; i++)
					{
						bool bSelected = (curProjectionTypeStr == projectionTypeStrs[i]);
						if (ImGui::Selectable(projectionTypeStrs[i],bSelected))
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
					if (ImGui::DragFloat("Fov",&fov))
					{
						camera.SetPerspectiveFov(glm::radians(fov));
					}

					float nearclip = camera.GetPerspectiveNear();
					if (ImGui::DragFloat("NearClip",&nearclip))
					{
						camera.SetPerspectiveNear(nearclip);
					}

					float farclip = camera.GetPerspectiveFar();
					if (ImGui::DragFloat("FarClip",&farclip))
					{
						camera.SetPerspectiveFar(farclip);
					}
				}
				else if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float size = camera.GetOrthophicSize();
					if (ImGui::DragFloat("Size",&size))
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

				ImGui::TreePop();
			}
			
			if (bRemove)
			{
				entity.RemoveComponent<CameraComponent>();
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