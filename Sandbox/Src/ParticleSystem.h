#pragma once
#include "Explore.h"

struct ParticleProps
{
	glm::vec2 Position;
	glm::vec2 Velocity, VelocityVariation;
	glm::vec4 ColorBegin, ColorEnd;
	float SizeBegin, SizeEnd, SizeVariation;
	float Lifetime = 1.0f;
};

class ParticleSystem
{
public:
	ParticleSystem();

	void OnUpdate(Explore::Timestep ts);

	void OnRender(Explore::OrthographicCamera& camera);

	void Emit(const ParticleProps& particleProps);

private:
	struct Particle
	{
		glm::vec2 Position;
		float Rotation = 0.0f;
		glm::vec2 Velocity;
		glm::vec4 ColorBegin, ColorEnd;
		float SizeBegin, SizeEnd;
		float LifeTime = 1.0f;
		float LifeRemaining = 0.0f;
		bool Active = false; 
	};

	std::vector<Particle> m_ParticlePool;

	uint32_t m_PoolIndex = 0;
};
