#pragma once
#include "Explore/Renderer/Camera.h"

namespace Explore
{
	/*
	*@ brief: a camera class, and supply interfaces that can change projection parameters 
	*/
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType : uint8_t
		{
			Perspective = 0,
			Orthographic = 1
		};
		
	public:
		SceneCamera()
		{
			ReCalculateProjection();
		}

		void SetProjectionType(ProjectionType projectionType) { m_Projectiontype = projectionType; ReCalculateProjection(); }

		ProjectionType GetProjectionType() { return m_Projectiontype; }

		void SetViewportSize(float width, float heigth);

		float GetAspectRatio() { return m_AspectRatio; }

		//Perspective
		void SetPerspective(float fov, float nearclip, float farclip);

		void SetPerspectiveFov(float perspectiveFov) { m_PerspectiveFov = perspectiveFov; ReCalculateProjection(); }

		float GetPerspectiveFov() { return m_PerspectiveFov; }

		void SetPerspectiveNear(float perspectiveNear) { m_PerspectiveNear = perspectiveNear; ReCalculateProjection(); }

		float GetPerspectiveNear() { return m_PerspectiveNear; }

		void SetPerspectiveFar(float perspectiveFar) { m_PerspectiveFar = perspectiveFar; ReCalculateProjection(); }

		float GetPerspectiveFar() { return m_PerspectiveFar; }

		//Orthographic
		void SetOrthographic(float size, float nearclip, float farclip);

		void SetOrthographicSize(float size) { m_OrthographicSize = size; ReCalculateProjection(); }
		
		float GetOrthophicSize() { return m_OrthographicSize; }

		void SetOrthographicNear(float orthographicNear) { m_OrthographicNear = orthographicNear; ReCalculateProjection(); }

		float GetOrthographicNear() { return m_OrthographicNear; }

		void SetOrthographicFar(float orthographicFar) { m_OrthographicFar = orthographicFar; ReCalculateProjection(); }

		float GetOrthographicFar() { return m_OrthographicFar; }

	private:
		void ReCalculateProjection();

	private:
		ProjectionType m_Projectiontype = ProjectionType::Orthographic;

		float m_AspectRatio = 1.0f;

		float m_PerspectiveFov = glm::radians(45.0f);

		float m_PerspectiveNear = 0.1f, m_PerspectiveFar = 100.0f;

		float m_OrthographicSize = 10.0f;

		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
	};
}