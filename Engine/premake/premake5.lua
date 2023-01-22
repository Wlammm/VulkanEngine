workspace "Engine"
    configurations { "Debug", "Release" }
    location "../"
    startproject "Launcher"
    language "C++"
    cppdialect "C++20"

project "Launcher"
    kind "ConsoleApp"
	location "../Launcher"
	files = { "**.cpp", "**.h", "**.hpp" }

project "Engine"
    kind "StaticLib"
	location "../Engine"

project "Editor"
    kind "StaticLib"
	location "../Editor"