#pragma once
#include "Explore/Renderer/Camera.h"

namespace Explore
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera() 
			: m_OrthographicSize(10.0f), m_OrthographicNear(-1.0f), m_OrthographicFar(1.0f), m_OrthographicRatio(1.0f) 
		{
			ReCalculateProjection();
		}

		void SetOrthographicSize(float size);

		float& GetOrthophicSize() { return m_OrthographicSize; }

		void SetViewportSize(float width, float heigth);

		float GetOrthographicRatio() { return m_OrthographicRatio; }

	private:
		void ReCalculateProjection();

	private:
		float m_OrthographicSize;

		float m_OrthographicNear, m_OrthographicFar;

		float m_OrthographicRatio;
	};
}