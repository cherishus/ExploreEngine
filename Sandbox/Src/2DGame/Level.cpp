#include "Level.h"
#include "Random.h"
#include <glm/gtc/matrix_transform.hpp>

static glm::vec4 HSVtoRGB(const glm::vec3& hsv) {
	int H = (int)(hsv.x * 360.0f);
	double S = hsv.y;
	double V = hsv.z;

	double C = S * V;
	double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	double m = V - C;
	double Rs, Gs, Bs;

	if (H >= 0 && H < 60) {
		Rs = C;
		Gs = X;
		Bs = 0;
	}
	else if (H >= 60 && H < 120) {
		Rs = X;
		Gs = C;
		Bs = 0;
	}
	else if (H >= 120 && H < 180) {
		Rs = 0;
		Gs = C;
		Bs = X;
	}
	else if (H >= 180 && H < 240) {
		Rs = 0;
		Gs = X;
		Bs = C;
	}
	else if (H >= 240 && H < 300) {
		Rs = X;
		Gs = 0;
		Bs = C;
	}
	else {
		Rs = C;
		Gs = 0;
		Bs = X;
	}

	return { (Rs + m), (Gs + m), (Bs + m), 1.0f };
}

static bool PointInTri(const glm::vec2& p, glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2)
{
	float s = p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y;
	float t = p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y;

	if ((s < 0) != (t < 0))
		return false;

	float A = -p1.y * p2.x + p0.y * (p2.x - p1.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y;

	return A < 0 ?
		(s <= 0 && s + t >= A) :
		(s >= 0 && s + t <= A);
}

void Level::Init()
{
	//load texture
	m_TriangleTexture = Explore::Texture2D::Create("assets/textures/Triangle.png");

	//init player
	m_Player.LoadAssets();

	//init map
	m_Pillars.resize(5);
	for (int i = 0; i < 5; i++)
	{
		CreatePillar(i, i * 10);
	}
}

void Level::OnUpdate(Timestep ts)
{
	m_Player.OnUpdate(ts);
	
	if (CollisionTest())
	{
		GameOver();
		return;
	}
	
	//update map
	if (m_Player.GetLocation().x > m_PillarTarget)
	{
		CreatePillar(m_PillarIndex, m_PillarTarget + 30.0f);
		m_PillarIndex = ++m_PillarIndex % m_Pillars.size();
		m_PillarTarget += 10.0f;
	}
}

void Level::OnRender()
{
	auto& location = m_Player.GetLocation();
	glm::vec4 color = HSVtoRGB(m_PillarHSV);

	//background
	Explore::Renderer2D::DrawQuad({ location.x,0.0f,-0.8f }, 0, { 50.0f,50.0f }, { 0.3f, 0.3f, 0.3f, 1.0f });

	//ceiling and floor
	Explore::Renderer2D::DrawQuad({ location.x, 34.0f }, 0, { 50.0f,50.0f }, color);
	Explore::Renderer2D::DrawQuad({ location.x, -34.0f }, 0, { 50.0f,50.0f }, color);

	//triangle
	for (auto& pillar : m_Pillars)
	{
		Explore::Renderer2D::DrawQuad(pillar.TopPosition, glm::radians(180.0f), pillar.TopScale, color, m_TriangleTexture);
		Explore::Renderer2D::DrawQuad(pillar.BottomPosition, 0, pillar.BottomScale, color, m_TriangleTexture);
	}

	//player
	m_Player.OnRender();
}

void Level::Reset()
{
	m_GameOver = false;

	m_PillarTarget = 20.0f;
	m_PillarIndex = 0;
	for (int i = 0; i < 5; i++)
	{
		CreatePillar(i, i * 10);
	}

	m_Player.Reset();
}

void Level::CreatePillar(int index, float offset)
{
	Pillar& pillar = m_Pillars[index];
	pillar.TopPosition.x = offset;
	pillar.BottomPosition.x = offset;
	pillar.TopPosition.z = index * 0.1f - 0.5f;
	pillar.BottomPosition.z = index * 0.1f - 0.5f;

	//random
	float center = Random::Float() * 6.0f - 3.0f;
	float gap = 2.0f + Random::Float() * 3.0f;

	pillar.TopPosition.y = center + gap * 0.5 + 10.0f;
	pillar.BottomPosition.y = center - gap * 0.5 - 10.0f;
}

bool Level::CollisionTest()
{
	auto& location = m_Player.GetLocation();
	
	//ceiling and floor collision test
	if (glm::abs(location.y) > 8.5f)
	{
		return true;
	}

	glm::vec4 playerPos[4] = {
		{-0.5f,-0.5f,0.0f,1.0f},
		{0.5f,-0.5f,0.0f,1.0f},
		{0.5f,0.5f,0.0f,1.0f},
		{-0.5f,-0.5f,0.0f,1.0f} 
	};

	glm::vec4 playerTransformPos[4];
	for (int i = 0; i < 4; i++)
	{
		playerTransformPos[i] = glm::translate(glm::mat4(1.0f), { location.x,location.y,0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Player.GetRotation()), { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { 1.0f,1.0f,1.0f })
			* playerPos[i];
	}

	glm::vec4 trianglePos[3] = {
		{-0.5f,-0.5f,0.0f,1.0f},
		{0.5f,-0.5f,0.0f,1.0f},
		{0.0f,0.45f,0.0f,1.0f}
	};

	for (auto& p : m_Pillars)
	{
		glm::vec2 tri[3];

		// Top pillars collision test
		for (int i = 0; i < 3; i++)
		{
			tri[i] = glm::translate(glm::mat4(1.0f), { p.TopPosition.x, p.TopPosition.y, 0.0f })
				* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { p.TopScale.x, p.TopScale.y, 1.0f })
				* trianglePos[i];
		}

		for (auto& vert : playerTransformPos)
		{
			if (PointInTri({ vert.x, vert.y }, tri[0], tri[1], tri[2]))
				return true;
		}

		// Bottom pillars collision test
		for (int i = 0; i < 3; i++)
		{
			tri[i] = glm::translate(glm::mat4(1.0f), { p.BottomPosition.x, p.BottomPosition.y, 0.0f })
				* glm::scale(glm::mat4(1.0f), { p.BottomScale.x, p.BottomScale.y, 1.0f })
				* trianglePos[i];
		}

		for (auto& vert : playerTransformPos)
		{
			if (PointInTri({ vert.x, vert.y }, tri[0], tri[1], tri[2]))
				return true;
		}
	}

	return false;
}

void Level::GameOver()
{
	m_GameOver = true;
}
