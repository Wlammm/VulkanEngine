VULKAN_SDK = os.getenv("VULKAN_SDK")

workspace "Engine"
    configurations { "Debug", "Release" }
	platforms { "Win64" }
    location "../"
    startproject "Launcher"
    language "C++"
    cppdialect "C++20"
	flags { "FatalWarnings" }
	debugdir "$(SolutionDir)../Bin/"
	targetdir  "../../Temp/"
	objdir  "../../Temp/"
	files 
	{ 
		"../%{prj.name}/**.cpp", 
		"../%{prj.name}/**.c", 
		"../%{prj.name}/**.hpp",
		"../%{prj.name}/**.h"
	}
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
	filter "configurations:Release"
		defines { "" }
		optimize "On"

project "Launcher"
    kind "ConsoleApp"
	location "../%{prj.name}"
	targetdir  "../../Bin/"
	includedirs
	{
		"%{VULKAN_SDK}/Include/",
		"$(SolutionDir)",
		"$(SolutionDir)External/",
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
	prebuildcommands { "cd Shaders", "CompileAllShaders.bat" }
	includedirs
	{
		"$(SolutionDir)",
		"$(ProjectDir)",
		"%{VULKAN_SDK}/Include/",
		"$(SolutionDir)External/",
	}
	links
	{
		"%{VULKAN_SDK}/Lib/vulkan-1.lib",
	}

project "Editor"
    kind "StaticLib"
	location "../%{prj.name}"
	
project "Unit Tests"
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
	}
	nuget
	{
		"Microsoft.googletest.v140.windesktop.msvcstl.static.rt-dyn:1.8.1.7",
	}