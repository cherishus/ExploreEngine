#include "Player.h"

void Player::LoadAssets()
{
	m_ShipTexture = Explore::Texture2D::Create("assets/textures/Ship.png");
}

void Player::OnUpdate(Explore::Timestep ts)
{
	m_Time += ts;

	if (Explore::Input::IsKeyPressed(EXPLORE_KEY_SPACE))
	{
		m_Velocity.y += m_EnginePower;
		if (m_Velocity.y < 0)
		{
			m_Velocity.y += m_EnginePower;
		}
	}
	else
	{
		m_Velocity.y -= m_Gravity;
	}
	m_Velocity.y = glm::clamp(m_Velocity.y, -10.0f, 10.0f);
	m_Location += m_Velocity * (float)ts;
}

void Player::OnRender()
{
	Explore::Renderer2D::DrawQuad(m_Location, glm::radians(GetRotation()), { 1.0f,1.0f }, m_ShipTexture);
}

void Player::Reset()
{
	m_Location = { -10.0f,0.0f };
	m_Velocity = { 5.0f, 0.0f };
}
