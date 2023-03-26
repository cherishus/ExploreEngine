#include "Sandbox2D.h"
#include "Platform/OpenGl/OpenGLShader.h"
#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1.7f, true)
{

}

void Sandbox2D::OnAttach()
{
	Explore::Renderer2D::Init();
	std::string path = "assets/textures/basketball.jpg";
	m_Texture = Explore::Texture2D::Create(path);

	//Init Particle
	m_Particle.ColorBegin = { 254.0f / 255.0f,212.0f / 255.0f,123.0f / 255.0f,1.0f };
	m_Particle.ColorEnd = { 254.0f / 255.0f, 109.0f / 255.0f, 41.0f / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.Lifetime = 1.0f;
	m_Particle.Velocity = { 0.0f,0.0f }, m_Particle.VelocityVariation = { 3.0f,1.0f };
	m_Particle.Position = { 0.0f,0.0f };

}

void Sandbox2D::OnDetach()
{
	Explore::Renderer2D::Shutdown();
}

void Sandbox2D::OnUpdate(Explore::Timestep ts)
{
	EXPLORE_PROFILE_FUNCTION()

	{
		EXPLORE_PROFILE_SCOPE("m_CameraController OnUpdate")
		m_CameraController.OnUpdate(ts);
	}

	{
		EXPLORE_PROFILE_SCOPE("Renderer Pre")
		Explore::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Explore::RenderCommand::Clear();
	}

	{
		EXPLORE_PROFILE_SCOPE("Renderer Draw")
		Explore::Renderer2D::ResetStats();
		Explore::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Explore::Renderer2D::DrawQuad({ -1.0f,0.5f,-0.1f },0, { 1.0f,1.0f }, m_Color);
		Explore::Renderer2D::DrawQuad({ 0.5f,0.5f,-0.1f }, 0,{ 1.0f,1.0f }, m_Color);
		Explore::Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f },0, { 5.0f,5.0f }, m_Texture,5.0f);
		Explore::Renderer2D::EndScene();
	}
	
	Explore::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Explore::Renderer2D::EndScene();

	if (Explore::Input::IsMouseButtonPressed(EXPLORE_MOUSE_BUTTON_LEFT))
	{
		auto mousePos = Explore::Input::GetMousePosition();
		auto width = Explore::Application::Get().GetWindow().GetWidth();
		auto height = Explore::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetLocation();
		auto x = (mousePos.first / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5;
		auto y = bounds.GetHeight() * 0.5 - (mousePos.second / height) * bounds.GetHeight();

		m_Particle.Position = { pos.x + x, pos.y + y };
		for (int i = 0; i< 5; i++)
		{
			m_ParticleSystem.Emit(m_Particle);
		}
	}
	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());
}

void Sandbox2D::OnImGuiRender()
{
	EXPLORE_PROFILE_FUNCTION()
	ImGui::Begin("Setting");
	//display draw statistics
	auto Stats = Explore::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Status:");
	ImGui::Text("Draw Calls: %d", Stats.DrawCalls);
	ImGui::Text("Draw Quads: %d", Stats.DrawQuads);
	//change color by UI
	ImGui::ColorEdit4("ColorPicker", glm::value_ptr(m_Color));
	ImGui::End();
}

void Sandbox2D::OnEvent(Explore::Event& event)
{
	m_CameraController.OnEvent(event);
}
