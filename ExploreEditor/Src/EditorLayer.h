#pragma once
#include "Explore.h"

namespace Explore 
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();

		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;

		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;

		virtual void OnImGuiRender() override;

		virtual void OnEvent(Event& event) override;

	private:
		OrthographicCameraController m_CameraController;

		Ref<Texture> m_Texture;

		Ref<SubTexture> m_SubTexture;

		glm::vec4 m_Color = { 1.0f,0.0f,0.0f,1.0f };

		Ref<FrameBuffer> m_FrameBuffer;

		glm::vec2 m_ViewportSize = {0,0};

		bool m_ViewportFocused = false;

		bool m_ViewportHovered = false;

		Ref<Scene> m_ActiveScene;

		Entity m_ActiveEntity;
	};
}