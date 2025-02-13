VULKAN_SDK = os.getenv("VULKAN_SDK")
EXTERNAL = "$(SolutionDir)External"

PHYSX_PATH = "../../PhysX/"

PHYSX_BIN_DIR = "../../PhysX/physx/bin/win.x86_64.vc143.mt/"
PHYSX_INCLUDE_DIR = "../../PhysX/physx/include/"
PHYSX_LIB_DIR = "%{PHYSX_BIN_DIR}/%{cfg.runtime}" -- Default to release
PHYSX_LIB_DIR_LIBSEARCH = "../../PhysX/physx/bin/win.x86_64.vc143.mt/debug/*.lib"



--filter { "configurations:*Debug*" }
--    PHYSX_LIB_DIR = "%{PHYSX_BIN_DIR}/debug"
--	PHYSX_LIB_DIR_LIBSEARCH = "../../PhysX/physx/bin/win.x86_64.vc143.mt/debug/*.lib"

filter {}

if not os.isdir(PHYSX_BIN_DIR) then 
	os.execute("cd internal_scripts/ && generate_physx_files.bat")
	os.execute("cd internal_scripts/ && compile_physx.bat")
else
	print("PhysX already compiled. Skipping")
end


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
    cppdialect "C++23"
	flags { "FatalWarnings" }
	debugdir "$(OutDir)"
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
		staticruntime "on"
	filter "configurations:Game Release"
		defines { "NDEBUG" }
		optimize "On"
		symbols "On"
		runtime "Release"
		staticruntime "on"
	filter "configurations:Editor Debug"
		defines { "DEBUG", "EDITOR" }
		symbols "On"
		runtime "Debug"
		staticruntime "on"
	filter "configurations:Editor Release"
		defines { "EDITOR", "NDEBUG" }
		optimize "On"
		symbols "On"
		runtime "Release"
		staticruntime "on"
		
	filter "configurations:PROFILE Game Debug"
		defines { "DEBUG", "TRACY_ENABLE", "TRACY_FIBERS" }
		symbols "On"
		runtime "Debug"
		editandcontinue "Off"
		staticruntime "on"
	filter "configurations:PROFILE Game Release"
		defines { "TRACY_ENABLE", "NDEBUG", "TRACY_FIBERS" }
		optimize "On"
		symbols "On"
		runtime "Release"
		editandcontinue "Off"
		staticruntime "on"
	filter "configurations:PROFILE Editor Debug"
		defines { "DEBUG", "EDITOR", "TRACY_ENABLE", "TRACY_FIBERS" }
		symbols "On"
		runtime "Debug"
		editandcontinue "Off"
		staticruntime "on"
	filter "configurations:PROFILE Editor Release"
		defines { "EDITOR", "TRACY_ENABLE", "NDEBUG", "TRACY_FIBERS" }
		optimize "On"
		symbols "On"
		runtime "Release"
		editandcontinue "Off"
		staticruntime "on"
		
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
		"%{PHYSX_INCLUDE_DIR}",
	}
	libdirs
	{
		"%{PHYSX_LIB_DIR}",
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
	
	-- Collect all .lib files in the PhysX library directory
	local physxLibs = os.matchfiles(PHYSX_LIB_DIR_LIBSEARCH)

	-- Extract only the file names
	local physxLibNames = {}
	for _, lib in ipairs(physxLibs) do
		table.insert(physxLibNames, path.getname(lib))
	end

	-- Link all found .lib files
	links(physxLibNames)
	
	for _, lib in ipairs(physxLibNames) do
		print("Linking PhysX lib: " .. lib)
	end

	postbuildcommands
	{
		"echo Running post build",
		"{COPY} %{PHYSX_LIB_DIR}/*.dll ../../Bin/"
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
		"%{PHYSX_INCLUDE_DIR}",
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
	filter { "configurations:*Debug*" }
		runtime "Debug"
	filter { "configurations:*Release*" }
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
	
