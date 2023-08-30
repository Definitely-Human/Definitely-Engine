workspace "Defen"
	architecture "x64"
	startproject "Sandbox"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Defen/vendor/GLFW/include"
IncludeDir["Glad"] = "Defen/vendor/Glad/include"
IncludeDir["ImGui"] = "Defen/vendor/imgui"
IncludeDir["glm"] = "Defen/vendor/glm"
IncludeDir["stb_image"] = "Defen/vendor/stb_image"

include "Defen/vendor/GLFW"
include "Defen/vendor/Glad"
include "Defen/vendor/imgui"

project "Defen"
	location "Defen"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "depch.h"
	pchsource "Defen/src/depch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"DE_PLATFORM_WINDOWS",
			"DE_BUILD_DLL",
		}

	filter "configurations:Debug"
		defines
		{
			"DE_DEBUG",
			"DE_ENABLE_ASSERTS"
		}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "DE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Debug"
		defines "DE_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Defen/vendor/spdlog/include",
		"Defen/src",
		"Defen/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Defen"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"DE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "DE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "DE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Debug"
		defines "DE_DIST"
		runtime "Release"
		optimize "on"