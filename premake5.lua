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
IncludeDir["GLAD"] = "ExploreEngine/ThirdLib/glad/include"
IncludeDir["ImGui"] = "ExploreEngine/ThirdLib/imgui"
IncludeDir["glm"] = "ExploreEngine/ThirdLib/glm"

include "ExploreEngine/ThirdLib/glfw"
include "ExploreEngine/ThirdLib/glad"
include "ExploreEngine/ThirdLib/imgui"

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
		"%{prj.name}/Src/**.cpp",
		"%{prj.name}/ThirdLib/glm/glm/**.hpp",
		"%{prj.name}/ThirdLib/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/Src",
		"%{prj.name}/ThirdLib/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
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
			"EXPLORE_ENABLE_ASSERTS",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "Explore_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "Explore_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "Explore_DIST"
		buildoptions "/MD"
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
		"ExploreEngine/Src",
		"%{IncludeDir.glm}"
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