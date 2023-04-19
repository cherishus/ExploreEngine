#include "enginepch.h"
#include "SceneCamera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Explore
{
	void SceneCamera::SetOrthographicSize(float size)
	{
		m_OrthographicSize = size;
		ReCalculateProjection();
	}

	void SceneCamera::SetViewportSize(float width, float heigth)
	{
		m_OrthographicRatio = width / heigth;
		ReCalculateProjection();
	}

	void SceneCamera::ReCalculateProjection()
	{
		float orthoLeft = -0.5 * m_OrthographicSize * m_OrthographicRatio;
		float orthoRight = 0.5 * m_OrthographicSize * m_OrthographicRatio;
		float orthoBottom = -0.5 * m_OrthographicSize;
		float orthoUp = 0.5 * m_OrthographicSize;
		m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoUp, m_OrthographicNear, m_OrthographicFar);
	}
}