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


#ifdef EXPLORE_ENABLE_ASSERTS
	#define EXPLORE_ASSERT(x,...) { if(!(x)) {EXPLORE_CLIENT_LOG(error,"Assertion Failed: {0}",__VA_ARGS__);__debugbreak();}}
	#define EXPLORE_CORE_ASSERT(x,...) {if(!(x)) {EXPLORE_CORE_LOG(error,"Assertion Failed: {0}",__VA_ARGS__);__debugbreak();}}
#else
	#define EXPLORE_ASSERT(x,...)
	#define EXPLORE_CORE_ASSERT(x,...)
#endif

#define BIT(x) (1 << (x))

#define EXPLORE_BIND_EVENT_FN(fn) std::bind(&fn,this,std::placeholders::_1) 