#pragma once
#include "glm/glm.hpp"

namespace Explore
{
	class Camera
	{
	public:
		Camera() : m_Projection(glm::mat4(1.0f)) {}

		Camera(const glm::mat4& projection) : m_Projection(projection) {}

		virtual ~Camera() = default;

		glm::mat4 GetProjectionMat() { return m_Projection; }

	protected:
		glm::mat4 m_Projection;
	};
}