#pragma once
#include "Explore/Renderer/OrthographicCamera.h"
#include "Explore/Core/Timestep.h"
#include "Explore/Events/MouseEvent.h"
#include "Explore/Events/ApplicationEvent.h"

namespace Explore
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool bRotation = false);

		void OnUpdate(Timestep ts); //be called in tick

		void OnEvent(Event& e); //deal with event

		void SetCamera(const OrthographicCamera& camera) { m_Camera = camera; }

		OrthographicCamera GetCamera() { return m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);

		bool OnWindowResize(WindowResizeEvent& e);

	private:
		float m_AspectRatio; //the ratio of width and height

		float m_ZoomLevel = 1.0f; //zoom level represents the height

		OrthographicCamera m_Camera; //orthographic camera

		bool m_Rotation; //whether control camera rotation or not

		glm::vec3 m_CameraLocation = { 0.0f,0.0f,0.0f };

		float m_CameraRotation = 0.0f;

		float m_CameraMoveSpeed = 1.0f;

		float m_CameraRotateSpeed = 20.0f;
	};
}