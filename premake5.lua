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

include "Defen/vendor/GLFW"
include "Defen/vendor/Glad"
include "Defen/vendor/imgui"

project "Defen"
	location "Defen"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "depch.h"
	pchsource "Defen/src/depch.cpp"
	
	
	staticruntime "off"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"${prj.name}/vendor/glm/glm/**.hpp",
		"${prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"DE_PLATFORM_WINDOWS",
			"DE_BUILD_DLL",
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines 
		{
			"DE_DEBUG",
			"DE_ENABLE_ASSERTS"
		}
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "DE_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Debug"
		defines "DE_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	staticruntime "off"
	
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
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"DE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "DE_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "DE_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Debug"
		defines "DE_DIST"
		runtime "Release"
		optimize "On"