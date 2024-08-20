VULKAN_SDK = os.getenv("VULKAN_SDK")
EXTERNAL = "$(SolutionDir)External"

workspace "Engine"
    configurations 
	{ 
		"Game Debug", "Game Release", "Editor Debug", "Editor Release", 
		"PROFILE Game Debug", "PROFILE Game Release", "PROFILE Editor Debug", "PROFILE Editor Release" 
	}
	platforms { "Win64" }
    location "../"
    startproject "Launcher"
    language "C++"
    cppdialect "C++20"
	flags { "FatalWarnings" }
	debugdir "$(SolutionDir)../Bin/"
	targetdir  "../../Temp/$(Configuration)/"
	objdir  "../../Temp/$(Configuration)/"
	files 
	{ 
		"../%{prj.name}/**.cpp", 
		"../%{prj.name}/**.c", 
		"../%{prj.name}/**.hpp",
		"../%{prj.name}/**.h",
		"../%{prj.name}/**.natvis",
	}
	
	filter "configurations:Game Debug"
		defines { "DEBUG" }
		symbols "On"
		runtime "Debug"
	filter "configurations:Game Release"
		defines { "" }
		optimize "On"
		symbols "On"
		runtime "Release"
	filter "configurations:Editor Debug"
		defines { "DEBUG", "EDITOR" }
		symbols "On"
		runtime "Debug"
	filter "configurations:Editor Release"
		defines { "EDITOR" }
		optimize "On"
		symbols "On"
		runtime "Release"
		
	filter "configurations:PROFILE Game Debug"
		defines { "DEBUG", "TRACY_ENABLE" }
		symbols "On"
		runtime "Debug"
		editandcontinue "Off"
	filter "configurations:PROFILE Game Release"
		defines { "TRACY_ENABLE" }
		optimize "On"
		symbols "On"
		runtime "Release"
		editandcontinue "Off"
	filter "configurations:PROFILE Editor Debug"
		defines { "DEBUG", "EDITOR", "TRACY_ENABLE" }
		symbols "On"
		runtime "Debug"
		editandcontinue "Off"
	filter "configurations:PROFILE Editor Release"
		defines { "EDITOR", "TRACY_ENABLE" }
		optimize "On"
		symbols "On"
		runtime "Release"
		editandcontinue "Off"
		
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
		"%{EXTERNAL}/aftermath/lib/GFSDK_Aftermath_Lib.x64.lib",
		"%{VULKAN_SDK}/Lib/shaderc_shared.lib",
		"%{VULKAN_SDK}/Lib/shaderc_util.lib",
		"ImGui",
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
		"$(SolutionDir)External/",
		"%{EXTERNAL}/assimp/include/",
		"%{EXTERNAL}/aftermath/include/",
		"$(SolutionDir)ImGui/",
		"%{EXTERNAL}/tracy/",
		"%{EXTERNAL}/glm/",
	}
	filter "files:../Engine/Tracy/TracyClient.cpp"
		flags { "NoPCH" }

project "Editor"
    kind "StaticLib"
	location "../%{prj.name}"
	pchheader "EditorPch.h"
	pchsource "../Editor/EditorPch.cpp"
	includedirs
	{
		"$(SolutionDir)",
		"$(ProjectDir)",
		"%{VULKAN_SDK}/Include/",
		"$(SolutionDir)External/",
		"%{EXTERNAL}/assimp/include/",
		"%{EXTERNAL}/aftermath/include/",
		"$(SolutionDir)ImGui/",
		"%{EXTERNAL}/tracy/",
		"%{EXTERNAL}/glm/",
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
	
