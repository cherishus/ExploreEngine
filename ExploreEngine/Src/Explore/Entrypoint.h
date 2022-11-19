#pragma once

#ifdef EXPLORE_PLATFORM_WINDOWS

extern Explore::Application* Explore::CreateApplication();

int main(int argc, char** argv)
{
	auto App = Explore::CreateApplication();
	App->Run();
	delete App;
	return 1;
}

#endif // !EXPLORE_PLATFORM_WINDOWS
