#include "enginepch.h"
#include "OrthographicCamera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Explore
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_Location({0.0f,0.0f,0.0f}),m_Rotation(0.0f),m_ViewMatrix(glm::mat4(1.0f))
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::ReCalculateViewMatrix()
	{
		//camera transform
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Location);
		transform = glm::rotate(transform, glm::radians(m_Rotation), { 0.0f,0.0f,1.0f });
		//update matrix
		m_ViewMatrix = glm::inverse(transform); //simulate camera by transforming all objects in the scene inversely
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}