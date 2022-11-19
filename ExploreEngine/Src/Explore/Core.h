#pragma once

#ifdef  EXPLORE_PLATFORM_WINDOWS
	#ifdef EXPLORE_BUILD_DLL
		#define EXPLORE_API __declspec(dllexport)
	#else
		#define EXPLORE_API __declspec(dllimport)
	#endif // EXPLORE_BUILD_DLL
#else
	#error Explore only support Windows
#endif //  EXPLORE_PLATFORM_WINDOWS