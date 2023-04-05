#include "enginepch.h"
#include "OrthographicCameraController.h"
#include "Explore/Core/input.h"
#include "Explore/Core/KeyCode.h"

namespace Explore
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool bRotation)
		: m_AspectRatio(aspectRatio), m_Rotation(bRotation),
		  m_Bounds(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		  m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top)
	{
		
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		EXPLORE_PROFILE_FUNCTION()
		//control camera by input polling
		if (Explore::Input::IsKeyPressed(EXPLORE_KEY_A))
		{
			m_CameraLocation.x -= m_CameraMoveSpeed * ts;
		}
		if (Explore::Input::IsKeyPressed(EXPLORE_KEY_D))
		{
			m_CameraLocation.x += m_CameraMoveSpeed * ts;
		}
		if (Explore::Input::IsKeyPressed(EXPLORE_KEY_S))
		{
			m_CameraLocation.y -= m_CameraMoveSpeed * ts;
		}
		if (Explore::Input::IsKeyPressed(EXPLORE_KEY_W))
		{
			m_CameraLocation.y += m_CameraMoveSpeed * ts;
		}
		m_Camera.SetLocation(m_CameraLocation);

		if (m_Rotation)
		{
			if (Explore::Input::IsKeyPressed(EXPLORE_KEY_Q))
			{
				m_CameraRotation += m_CameraRotateSpeed * ts;
			}
			if (Explore::Input::IsKeyPressed(EXPLORE_KEY_E))
			{
				m_CameraRotation -= m_CameraRotateSpeed * ts;
			}
			m_Camera.SetRotaion(m_CameraRotation);
		}
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher ed(e);
		ed.Dispatch<MouseScrolledEvent>(EXPLORE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		ed.Dispatch<WindowResizeEvent>(EXPLORE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResize));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		if (height != 0)
		{
			m_AspectRatio = width / height;
		}
		else
		{
			m_AspectRatio = 1.0f;
		}
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset();
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
		return false;
	}

	bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& e)
	{
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
}