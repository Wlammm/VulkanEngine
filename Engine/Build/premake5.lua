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
		staticruntime "on"
		runtime "Debug"
	filter "configurations:Game Release"
		defines { "" }
		optimize "On"
		symbols "On"
		staticruntime "on"
		runtime "Release"
	filter "configurations:Editor Debug"
		defines { "DEBUG", "EDITOR" }
		symbols "On"
		staticruntime "on"
		runtime "Debug"
	filter "configurations:Editor Release"
		defines { "EDITOR" }
		optimize "On"
		symbols "On"
		staticruntime "on"
		runtime "Release"
		
	filter "configurations:PROFILE Game Debug"
		defines { "DEBUG", "TRACY_ENABLE" }
		symbols "On"
		runtime "Debug"
		staticruntime "on"
		editandcontinue "Off"
	filter "configurations:PROFILE Game Release"
		defines { "TRACY_ENABLE" }
		optimize "On"
		symbols "On"
		runtime "Release"
		staticruntime "on"
		editandcontinue "Off"
	filter "configurations:PROFILE Editor Debug"
		defines { "DEBUG", "EDITOR", "TRACY_ENABLE" }
		symbols "On"
		runtime "Debug"
		staticruntime "on"
		editandcontinue "Off"
	filter "configurations:PROFILE Editor Release"
		defines { "EDITOR", "TRACY_ENABLE" }
		optimize "On"
		symbols "On"
		runtime "Release"
		staticruntime "on"
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
		"%{EXTERNAL}/mimalloc/Lib/mimalloc-static.lib",
		"Engine",
		"Editor",
		"%{VULKAN_SDK}/Lib/vulkan-1.lib",
		"%{EXTERNAL}/assimp/lib/assimp-vc143-mt.lib",
		"%{EXTERNAL}/aftermath/lib/GFSDK_Aftermath_Lib.x64.lib",
		"%{VULKAN_SDK}/Lib/shaderc_shared.lib",
		"%{VULKAN_SDK}/Lib/shaderc_util.lib",
		"ImGui",
	}
	
	filter "configurations:*Release*"
		links
		{
			"%{EXTERNAL}/PhysX/lib/Release/LowLevel_static_64.lib",
			"%{EXTERNAL}/PhysX/lib/Release/LowLevelAABB_static_64.lib",
			"%{EXTERNAL}/PhysX/lib/Release/LowLevelDynamics_static_64.lib",
			"%{EXTERNAL}/PhysX/lib/Release/PhysX_64.lib",
			"%{EXTERNAL}/PhysX/lib/Release/PhysXCharacterKinematic_static_64.lib",
			"%{EXTERNAL}/PhysX/lib/Release/PhysXCommon_64.lib",
			"%{EXTERNAL}/PhysX/lib/Release/PhysXCooking_64.lib",
			"%{EXTERNAL}/PhysX/lib/Release/PhysXExtensions_static_64.lib",
			"%{EXTERNAL}/PhysX/lib/Release/PhysXFoundation_64.lib",
			"%{EXTERNAL}/PhysX/lib/Release/PhysXPvdSDK_static_64.lib",
			"%{EXTERNAL}/PhysX/lib/Release/PhysXTask_static_64.lib",
			"%{EXTERNAL}/PhysX/lib/Release/PhysXVehicle_static_64.lib",
			"%{EXTERNAL}/PhysX/lib/Release/PhysXVehicle2_static_64.lib",
			"%{EXTERNAL}/PhysX/lib/Release/SceneQuery_static_64.lib",
			"%{EXTERNAL}/PhysX/lib/Release/SimulationController_static_64.lib",
		}
		
	filter "configurations:*Debug*"
		links
		{
			"%{EXTERNAL}/PhysX/lib/Debug/LowLevel_static_64.lib",
			"%{EXTERNAL}/PhysX/lib/Debug/LowLevelAABB_static_64.lib",
			"%{EXTERNAL}/PhysX/lib/Debug/LowLevelDynamics_static_64.lib",
			"%{EXTERNAL}/PhysX/lib/Debug/PhysX_64.lib",
			"%{EXTERNAL}/PhysX/lib/Debug/PhysXCharacterKinematic_static_64.lib",
			"%{EXTERNAL}/PhysX/lib/Debug/PhysXCommon_64.lib",
			"%{EXTERNAL}/PhysX/lib/Debug/PhysXCooking_64.lib",
			"%{EXTERNAL}/PhysX/lib/Debug/PhysXExtensions_static_64.lib",
			"%{EXTERNAL}/PhysX/lib/Debug/PhysXFoundation_64.lib",
			"%{EXTERNAL}/PhysX/lib/Debug/PhysXPvdSDK_static_64.lib",
			"%{EXTERNAL}/PhysX/lib/Debug/PhysXTask_static_64.lib",
			"%{EXTERNAL}/PhysX/lib/Debug/PhysXVehicle_static_64.lib",
			"%{EXTERNAL}/PhysX/lib/Debug/PhysXVehicle2_static_64.lib",
			"%{EXTERNAL}/PhysX/lib/Debug/SceneQuery_static_64.lib",
			"%{EXTERNAL}/PhysX/lib/Debug/SimulationController_static_64.lib",
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
		"%{EXTERNAL}/mimalloc/include/",
		"%{EXTERNAL}/PerlinNoise/",
		"%{EXTERNAL}/PhysX/include/",
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
		"%{EXTERNAL}/PhysX/include/",
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
	staticruntime "on"
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
	
