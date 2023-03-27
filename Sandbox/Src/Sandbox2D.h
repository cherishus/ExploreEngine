#pragma once
#include "Explore.h"
#include "ParticleSystem.h"

class Sandbox2D : public Explore::Layer
{
public:
	Sandbox2D();

	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	
	virtual void OnDetach() override;
	
	virtual void OnUpdate(Explore::Timestep ts) override;
	
	virtual void OnImGuiRender() override;
	
	virtual void OnEvent(Explore::Event& event) override;
	 
private:
	Explore::OrthographicCameraController m_CameraController;

	Explore::Ref<Explore::Texture> m_Texture;

	Explore::Ref<Explore::SubTexture> m_SubTexture;

	glm::vec4 m_Color = { 1.0f,0.0f,0.0f,1.0f };

	ParticleProps m_Particle;

	ParticleSystem m_ParticleSystem;
};