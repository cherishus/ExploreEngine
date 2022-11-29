workspace "ExploreEngine"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "ExploreEngine/ThirdLib/glfw/include"

include "ExploreEngine/ThirdLib/glfw"

project "ExploreEngine"
	location "ExploreEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "enginepch.h"
	pchsource "enginepch.cpp"

	files
	{
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/Src",
		"%{prj.name}/ThirdLib/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"EXPLORE_PLATFORM_WINDOWS",
			"EXPLORE_BUILD_DLL",
			"EXPLORE_ENABLE_ASSERTS"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "Explore_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "Explore_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "Explore_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.cpp"
	}

	includedirs
	{
		"ExploreEngine/ThirdLib/spdlog/include",
		"ExploreEngine/Src"
	}

	links
	{
		"ExploreEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"EXPLORE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "Explore_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "Explore_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "Explore_DIST"
		optimize "On"