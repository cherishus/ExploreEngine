#pragma once

#ifdef EXPLORE_PLATFORM_WINDOWS

extern Explore::Application* Explore::CreateApplication();

int main(int argc, char** argv)
{
	Explore::Log::Init();
	EXPLORE_CORE_LOG(info, "Start Explore Engine!");
	EXPLORE_CORE_LOG(info, "Keep Passion and Enjoy Life!");

	auto App = Explore::CreateApplication();
	App->Run();
	delete App;
	return 1;
}

#endif // !EXPLORE_PLATFORM_WINDOWS
