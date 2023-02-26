#pragma once
#include "Explore.h"
#include "Player.h"

struct Pillar
{
	//top triangle
	glm::vec3 TopPosition = { 0.0f, 10.0f, 0.0f };

	glm::vec2 TopScale = { 15.0f, 20.0f };
	
	//bottom triangle
	glm::vec3 BottomPosition = { 0.0f, -10.0f, 0.0f };
	
	glm::vec2 BottomScale = { 15.0f, 20.0f };
};

class Level
{
public:
	void Init();

	void OnUpdate(Timestep ts);

	void OnRender();
	
	void Reset();

	Player& GetPlayer() { return m_Player; }

	bool IsGameOver() { return m_GameOver; }

private:
	void CreatePillar(int index, float offset);
	
	bool CollisionTest();

	void GameOver();

private:
	//player
	Player m_Player;

	//map
	std::vector<Pillar> m_Pillars;

	Explore::Ref<Explore::Texture> m_TriangleTexture;

	bool m_GameOver = false;

	float m_PillarTarget = 20.0f;

	int m_PillarIndex = 0;

	glm::vec3 m_PillarHSV = { 0.0f, 0.8f, 0.8f };
};