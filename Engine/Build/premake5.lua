VULKAN_SDK = os.getenv("VULKAN_SDK")
EXTERNAL = "$(SolutionDir)External"

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
		"../%{prj.name}/**.h",
		"../%{prj.name}/**.natvis",
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
		"$(SolutionDir)ImGui/",
	}
	
	links
	{
		"Engine",
		"Editor",
		"%{VULKAN_SDK}/Lib/vulkan-1.lib",
		"%{EXTERNAL}/assimp/lib/assimp-vc143-mt.lib",
		"ImGui",
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
		"%{EXTERNAL}/assimp/include/",
		"$(SolutionDir)ImGui/",
	}

project "Editor"
    kind "StaticLib"
	location "../%{prj.name}"
	includedirs
	{
		"$(SolutionDir)",
		"$(ProjectDir)",
		"%{VULKAN_SDK}/Include/",
		"$(SolutionDir)External/",
		"%{EXTERNAL}/assimp/include/",
		"$(SolutionDir)ImGui/",
	}
	
project "ImGui"
    kind "StaticLib"
	location "../%{prj.name}"
	includedirs
	{
		"$(ProjectDir)",
		"%{VULKAN_SDK}/Include/",
	}
	
project "Unit Test"
    kind "ConsoleApp"
	location "../%{prj.name}"
	pchheader "UnitTestPch.h"
	pchsource "../Unit Test/UnitTestPch.cpp"
	staticruntime "off"
	runtime "Release"
	links
	{
		"Engine",
	}
	includedirs
	{
		"$(SolutionDir)",
	}
	nuget
	{
		"Microsoft.googletest.v140.windesktop.msvcstl.static.rt-dyn:1.8.1.7"
	}