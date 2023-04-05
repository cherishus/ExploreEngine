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
IncludeDir["stb_image"] = "ExploreEngine/ThirdLib/stb_image"

group "Dependencies"
include "ExploreEngine/ThirdLib/glfw"
include "ExploreEngine/ThirdLib/glad"
include "ExploreEngine/ThirdLib/imgui"
group ""

project "ExploreEngine"
	location "ExploreEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "enginepch.h"
	pchsource "enginepch.cpp"

	files
	{
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.cpp",
		"%{prj.name}/ThirdLib/glm/glm/**.hpp",
		"%{prj.name}/ThirdLib/glm/glm/**.inl",
		"%{prj.name}/ThirdLib/stb_image/**.h",
		"%{prj.name}/ThirdLib/stb_image/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/Src",
		"%{prj.name}/ThirdLib/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"EXPLORE_PLATFORM_WINDOWS",
			"EXPLORE_BUILD_DLL",
			"EXPLORE_ENABLE_ASSERTS",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "Explore_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "Explore_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "Explore_DIST"
		runtime "Release"
		optimize "on"

project "ExploreEditor"
	location "ExploreEditor"
	kind "ConsoleApp"
	language "C++"
	staticruntime "on"

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
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"ExploreEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"EXPLORE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "Explore_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "Explore_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "Explore_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "on"

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
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"ExploreEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"EXPLORE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "Explore_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "Explore_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "Explore_DIST"
		runtime "Release"
		optimize "on"