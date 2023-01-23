#pragma once
#include "Explore.h"

class Sandbox2D : public Explore::Layer
{
public:
	Sandbox2D();

	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	
	virtual void OnDetach() override;
	
	virtual void OnUpdate(Timestep ts) override;
	
	virtual void OnImGuiRender() override;
	
	virtual void OnEvent(Explore::Event& event) override;
	 
private:
	Explore::OrthographicCameraController m_CameraController;

	Explore::Ref<Explore::Texture> m_Texture;

	glm::vec4 m_Color = { 1.0f,0.0f,0.0f,1.0f };
};