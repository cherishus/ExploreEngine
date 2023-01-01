#pragma once
#include "glm/glm.hpp"

namespace Explore
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left,float right,float bottom, float top);

		void SetLocation(const glm::vec3& location) { m_Location = location; ReCalculateViewMatrix(); }

		glm::vec3 GetLocation() { return m_Location; }

		void SetRotaion(float rotation) { m_Rotation = rotation; ReCalculateViewMatrix(); }

		float GetRotation() { return m_Rotation; }
		
		glm::mat4 GetViewMatrix() { return m_ViewMatrix; }
		
		glm::mat4 GetProjectionMatrix() { return m_ProjectionMatrix; }

		glm::mat4 GetViewProjectionMatrix() { return m_ViewProjectionMatrix; }
	
	private:
		void ReCalculateViewMatrix();

	private:
		glm::mat4 m_ViewMatrix; //calculated by camera transform
		 
		glm::mat4 m_ProjectionMatrix; //calculate by camera property

		glm::mat4 m_ViewProjectionMatrix; //p * v, the reason of p * v is column-major in glm

		glm::vec3 m_Location; //camera location

		float m_Rotation; //rotate angle by Z-Axis for 2D Renderer
	};
}