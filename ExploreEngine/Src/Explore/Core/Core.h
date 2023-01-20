#pragma once
#include <memory>
#include <functional>

#ifdef  EXPLORE_PLATFORM_WINDOWS
	#if HZ_DYNAMIC_LINK
		#ifdef EXPLORE_BUILD_DLL
			#define EXPLORE_API __declspec(dllexport)
		#else
			#define EXPLORE_API __declspec(dllimport)
		#endif // EXPLORE_BUILD_DLL
	#else
		#define EXPLORE_API
	#endif // HZ_DYNAMIC_LINK
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

//Ref & Scope definition
namespace Explore
{
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using Scope = std::unique_ptr<T>;
}