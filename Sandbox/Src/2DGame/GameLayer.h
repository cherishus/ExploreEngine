#pragma once
#include "Explore.h"
#include "Level.h"

class GameLayer : public Explore::Layer
{
public:
	GameLayer();

	virtual ~GameLayer() = default;

	virtual void OnAttach() override;

	virtual void OnDetach() override;

	virtual void OnUpdate(Timestep ts) override;

	virtual void OnImGuiRender() override;

	virtual void OnEvent(Explore::Event& event) override;

	bool OnMouseButtonPressed(Explore::MouseButtonPressedEvent& e);

	bool OnWindowResize(Explore::WindowResizeEvent& e);

private:
	void CreateCamera(float aspectRatio);

public:
	enum class GameState : uint8_t
	{
		Start = 0,
		Play = 1,
		Over = 2
	};

private:
	//camera
	Explore::Scope<Explore::OrthographicCamera> m_Camera;
	
	//level
	Level m_Level;

	GameState m_GameState = GameState::Start;

	float m_Time = 0.0f;
};