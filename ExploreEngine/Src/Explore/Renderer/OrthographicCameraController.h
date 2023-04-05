#pragma once
#include "Explore/Renderer/OrthographicCamera.h"
#include "Explore/Core/Timestep.h"
#include "Explore/Events/MouseEvent.h"
#include "Explore/Events/ApplicationEvent.h"

namespace Explore
{
	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		OrthographicCameraBounds(float L,float R,float B,float T):Left(L),Right(R),Bottom(B),Top(T) {}

		float GetWidth() { return (Right - Left); }
		
		float GetHeight() { return (Top - Bottom); }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool bRotation = false);

		void OnUpdate(Timestep ts); //be called in tick

		void OnEvent(Event& e); //deal with event

		void OnResize(float width, float height);

		void SetCamera(const OrthographicCamera& camera) { m_Camera = camera; }

		OrthographicCamera GetCamera() { return m_Camera; }

		const OrthographicCameraBounds& GetBounds() { return m_Bounds; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);

		bool OnWindowResize(WindowResizeEvent& e);

	private:
		float m_AspectRatio; //the ratio of width and height

		float m_ZoomLevel = 1.0f; //zoom level represents the height

		OrthographicCamera m_Camera; //orthographic camera

		OrthographicCameraBounds m_Bounds;

		bool m_Rotation; //whether control camera rotation or not

		glm::vec3 m_CameraLocation = { 0.0f,0.0f,0.0f };

		float m_CameraRotation = 0.0f;

		float m_CameraMoveSpeed = 1.0f;

		float m_CameraRotateSpeed = 20.0f;
	};
}