#pragma once
#include "Explore.h"

class Player
{
public:
	void LoadAssets();

	void OnUpdate(Timestep ts);

	void OnRender();

	void Reset();

	const glm::vec2& GetLocation() { return m_Location; }

	float GetRotation() { return m_Velocity.y * 4.0f - 90.0f; }

private:
	float m_Time = 0.0f;
	
	Explore::Ref<Explore::Texture> m_ShipTexture;

	glm::vec2 m_Location = { -10.0f,0.0f };

	glm::vec2 m_Velocity = { 5.0f, 0.0f };

	float m_Gravity = 0.4f;
	
	float m_EnginePower = 0.5f;
};