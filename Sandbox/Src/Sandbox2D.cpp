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
}

void Sandbox2D::OnDetach()
{
	Explore::Renderer2D::Shutdown();
}

void Sandbox2D::OnUpdate(Timestep ts)
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
