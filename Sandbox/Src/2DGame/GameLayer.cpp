#include "GameLayer.h"
#include "Random.h"

GameLayer::GameLayer() : Layer("GameLayer")
{
	auto& window = Explore::Application::Get().GetWindow();
	CreateCamera(window.GetHeight() != 0 ? window.GetWidth() / window.GetHeight() : 1.0f);

	Random::Init();
}

void GameLayer::OnAttach()
{
	m_Level.Init();
	Explore::Renderer2D::Init();
}

void GameLayer::OnDetach()
{
	Explore::Renderer2D::Shutdown();
}

void GameLayer::OnUpdate(Timestep ts)
{
	//game tick
	m_Time += ts;
	if (m_Level.IsGameOver())
	{
		m_GameState = GameState::Over;
	}
	if (m_GameState == GameState::Play)
	{
		m_Level.OnUpdate(ts);
		const auto& playerPos = m_Level.GetPlayer().GetLocation();
		m_Camera->SetLocation({ playerPos.x, playerPos.y, 0.0f });
	}

	//render tick
	Explore::RenderCommand::SetClearColor({ 0.0f,0.0f,0.0f,1.0f });
	Explore::RenderCommand::Clear();
	Explore::Renderer2D::BeginScene(*m_Camera);
	m_Level.OnRender();
	Explore::Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender()
{
	switch (m_GameState)
	{
		case GameState::Start:
		{

			break;
		}
		case GameState::Over:
		{
			break;
		}
	}
}

void GameLayer::OnEvent(Explore::Event& event)
{
	Explore::EventDispatcher ed(event);
	ed.Dispatch<Explore::MouseButtonPressedEvent>(EXPLORE_BIND_EVENT_FN(GameLayer::OnMouseButtonPressed));
	ed.Dispatch<Explore::WindowResizeEvent>(EXPLORE_BIND_EVENT_FN(GameLayer::OnWindowResize));
}

bool GameLayer::OnMouseButtonPressed(Explore::MouseButtonPressedEvent& e)
{
	if (m_GameState == GameState::Over)
	{
		m_Level.Reset();
	}
	m_GameState = GameState::Play;
	return false;
}

bool GameLayer::OnWindowResize(Explore::WindowResizeEvent& e)
{
	float aspectRatio = 1.0f;
	if (e.GetHeight() != 0)
	{
		aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
	}
	CreateCamera(aspectRatio);
	return false;
}

void GameLayer::CreateCamera(float aspectRatio)
{
	float cameraHeight = 8.0f;
	float bottom = -cameraHeight;
	float top = -bottom;
	float left = -cameraHeight * aspectRatio;
	float right = -left;
	m_Camera.reset(new Explore::OrthographicCamera(left, right, bottom, top));
}
