#include "Explore.h"

class ExampleLayer : public Explore::Layer
{
public:
	ExampleLayer() : Layer("ExampleLayer") {}

	void OnUpdate() override
	{
		//EXPLORE_CLIENT_LOG(info, "ExampleLayer::Update()");
	}

	void OnEvent(Explore::Event& event) override
	{
		EXPLORE_CLIENT_LOG(info, "Examplelayer::OnEvent()");
	}
};

class Sandbox : public Explore::Application
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer);
	}

	~Sandbox() {}
};

Explore::Application* Explore::CreateApplication()
{
	return new Sandbox();
}