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

		static void DrawVec3(const std::string& label, glm::vec3& value, float resetVal = 0.0f, float columnWidth = 100.0f);

	private:
		Ref<Scene> m_Context;

		Entity m_SelectedEntity;
	};
}