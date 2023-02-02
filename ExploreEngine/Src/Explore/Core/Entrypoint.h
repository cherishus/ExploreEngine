#pragma once

#ifdef EXPLORE_PLATFORM_WINDOWS

extern Explore::Application* Explore::CreateApplication();

int main(int argc, char** argv)
{
	Explore::Log::Init();
	EXPLORE_CORE_LOG(info, "Start Explore Engine!");
	auto App = Explore::CreateApplication();

	EXPLORE_PROFILE_BEGIN_SESSION("startup", "explore_profile_result.json")
	App->Run();
	EXPLORE_PROFILE_END_SESSION()

	delete App;
	return 1;
}

#endif // !EXPLORE_PLATFORM_WINDOWS
