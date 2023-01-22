workspace "Engine"
    configurations { "Debug", "Release" }
    location "../"
    startproject "Launcher"
    language "C++"
    cppdialect "C++20"

project "Launcher"
    kind "ConsoleApp"

project "Engine"
    kind "StaticLib"

project "Editor"
    kind "StaticLib"