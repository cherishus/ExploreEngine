#pragma once
#include "Explore.h"

namespace Explore
{
	/*
	* @brief: UI about SceneHierarchy Panel and Property Panel
	*/
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;

		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity& entity);

		void DrawComponentNode(Entity& entity);

		template<typename T, typename UIFunc>
		static void DrawComponent(const std::string& name, Entity entity, UIFunc function);
		
		//UI containing two columns, one is lable string, two is DragVec3
		static void DrawVec3(const std::string& label, glm::vec3& value, float resetVal = 0.0f, float columnWidth = 100.0f);

	private:
		Ref<Scene> m_Context;

		Entity m_SelectedEntity;
	};
}