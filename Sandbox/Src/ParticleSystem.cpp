#include "ParticleSystem.h"
#include <random>
#include "glm/gtc/constants.hpp"
#include "glm/gtx/compatibility.hpp"

class TempRandom
{
public:
	static void Init()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	static float Float()
	{
		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

private:
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};

std::mt19937 TempRandom::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> TempRandom::s_Distribution;

ParticleSystem::ParticleSystem()
{
	m_ParticlePool.resize(100);
}

void ParticleSystem::OnUpdate(Explore::Timestep ts)
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
		{
			continue;
		}
		particle.LifeRemaining -= ts;
		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}
		particle.Position += particle.Velocity * (float)ts;
		particle.Rotation += 0.5f * ts;
	}
}

void ParticleSystem::OnRender(Explore::OrthographicCamera& camera)
{
	Explore::Renderer2D::BeginScene(camera);
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
		{
			continue;
		}
		float life = particle.LifeRemaining / particle.LifeTime;
		glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
		Explore::Renderer2D::DrawQuad(particle.Position, particle.Rotation, { size,size }, color);
	}
	Explore::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.Active = true;
	particle.Position = particleProps.Position;
	particle.Rotation = TempRandom::Float() * 5.0f;

	//velocity
	particle.Velocity = particleProps.Velocity;
	particle.Velocity.x = particleProps.VelocityVariation.x * (TempRandom::Float() - 0.5f );
	particle.Velocity.y = particleProps.VelocityVariation.y * (TempRandom::Float() - 0.5f);

	//color
	particle.ColorBegin = particleProps.ColorBegin;
	particle.ColorEnd = particleProps.ColorEnd;

	particle.LifeTime = particleProps.Lifetime;
	particle.LifeRemaining = particleProps.Lifetime;
	particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (TempRandom::Float() - 0.5f);
	particle.SizeEnd = particleProps.SizeEnd;

	m_PoolIndex = ++m_PoolIndex % m_ParticlePool.size();
}