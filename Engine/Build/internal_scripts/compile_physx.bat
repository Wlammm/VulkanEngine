@echo off
setlocal enabledelayedexpansion

echo Running VSWHERE

:: Locate MSBuild using vswhere
for /F "usebackq tokens=*" %%i in (`vswhere -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
    set MSBUILD_PATH=%%i
)

:: Check if vc17win64 or vc16win64 exists
if exist "../../../PhysX/physx/compiler/vc17win64" (
    set SOLUTION_PATH=../../../PhysX/physx/compiler/vc17win64/PhysXSDK.sln
    set SOLUTION_DIR=../../../PhysX/physx/compiler/vc17win64
) else if exist "../../../PhysX/physx/compiler/vc16win64" (
    set SOLUTION_PATH=../../../PhysX/physx/compiler/vc16win64/PhysXSDK.sln
    set SOLUTION_DIR=../../../PhysX/physx/compiler/vc16win64
) else (
    echo No valid solution folder found!
    exit /b 1
)

:: Convert relative path to absolute path
for /F "delims=" %%i in ("%SOLUTION_DIR%") do set ABS_PATH=%%~fi

echo Found solution in: %ABS_PATH%
cd /d "%ABS_PATH%"

:: Run MSBuild
"%MSBUILD_PATH%" sdk_source_bin/PhysX.sln /p:Configuration=Release /p:Platform=x64
"%MSBUILD_PATH%" sdk_source_bin/PhysX.sln /p:Configuration=Debug /p:Platform=x64
"%MSBUILD_PATH%" pvdruntime_bin/PVDRuntime.sln /p:Configuration=Release /p:Platform=x64
"%MSBUILD_PATH%" pvdruntime_bin/PVDRuntime.sln /p:Configuration=Debug /p:Platform=x64