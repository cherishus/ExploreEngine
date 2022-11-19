#include "Explore.h"

class Sandbox : public Explore::Application
{
public:
	Sandbox() {}

	~Sandbox() {}
};

Explore::Application* Explore::CreateApplication()
{
	return new Sandbox();
}