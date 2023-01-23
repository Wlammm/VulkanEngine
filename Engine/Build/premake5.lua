workspace "Engine"
    configurations { "Debug", "Release" }
    location "../"
    startproject "Launcher"
    language "C++"
    cppdialect "C++20"
	files 
	{ 
		"../%{prj.name}/**.cpp", 
		"../%{prj.name}/**.c", 
		"../%{prj.name}/**.hpp",
		"../%{prj.name}/**.h"
	}
	outdir "../../Bin/"

project "Launcher"
    kind "ConsoleApp"
	location "../%{prj.name}"
	includedirs
	{
		"../"
	}
	
	links
	{
		"Engine"
	}

project "Engine"
    kind "StaticLib"
	location "../%{prj.name}"

project "Editor"
    kind "StaticLib"
	location "../%{prj.name}"