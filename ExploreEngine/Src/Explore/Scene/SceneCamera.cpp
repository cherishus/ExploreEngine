#include "enginepch.h"
#include "SceneCamera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Explore
{
	void SceneCamera::SetViewportSize(float width, float heigth)
	{
		m_AspectRatio = width / heigth;
		ReCalculateProjection();
	}

	void SceneCamera::SetPerspective(float fov, float nearclip, float farclip)
	{
		m_Projectiontype = ProjectionType::Perspective;
		m_PerspectiveFov = fov;
		m_PerspectiveNear = nearclip;
		m_PerspectiveFar = farclip;
		ReCalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearclip, float farclip)
	{
		m_Projectiontype = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearclip;
		m_OrthographicFar = farclip;
		ReCalculateProjection();
	}

	void SceneCamera::ReCalculateProjection()
	{
		if (m_Projectiontype == ProjectionType::Perspective)
		{
			m_Projection = glm::perspective(m_PerspectiveFov, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		else
		{
			float orthoLeft = -0.5 * m_OrthographicSize * m_AspectRatio;
			float orthoRight = 0.5 * m_OrthographicSize * m_AspectRatio;
			float orthoBottom = -0.5 * m_OrthographicSize;
			float orthoUp = 0.5 * m_OrthographicSize;
			m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoUp, m_OrthographicNear, m_OrthographicFar);
		}
	}
}