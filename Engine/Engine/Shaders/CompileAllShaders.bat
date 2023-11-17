@echo off
for %%i in (*.*) do if NOT %%i == %~n0%~x0 glslc.exe %%i -o ../../../Bin/CompiledShaders/%%i.spv
pause