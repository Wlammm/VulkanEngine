VULKAN_SDK = os.getenv("VULKAN_SDK")

workspace "Engine"
    configurations { "Debug", "Release" }
	platforms { "Win64" }
    location "../"
    startproject "Launcher"
    language "C++"
    cppdialect "C++20"
	targetdir  "../../Temp/"
	objdir  "../../Temp/"
	files 
	{ 
		"../%{prj.name}/**.cpp", 
		"../%{prj.name}/**.c", 
		"../%{prj.name}/**.hpp",
		"../%{prj.name}/**.h"
	}

project "Launcher"
    kind "ConsoleApp"
	location "../%{prj.name}"
	targetdir  "../../Bin/"
	includedirs
	{
		"$(SolutionDir)",
	}
	
	links
	{
		"Engine",
		"Editor",
	}

project "Engine"
    kind "StaticLib"
	location "../%{prj.name}"
	pchheader "EnginePch.h"
	pchsource "../Engine/EnginePch.cpp"
	includedirs
	{
		"$(SolutionDir)",
		"$(ProjectDir)",
		"%{VULKAN_SDK}/Include/",
	}
	links
	{
		"%{VULKAN_SDK}/Lib/vulkan-1.lib",
	}

project "Editor"
    kind "StaticLib"
	location "../%{prj.name}"