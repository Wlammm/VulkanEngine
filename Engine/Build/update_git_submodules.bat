@echo off
cd ..
cd ..
echo Updating submodules from github...

git submodule init
git submodule update
echo Update successful

cd Engine/imgui
rmdir /s /q examples
rmdir /s /q docs
rmdir /s /q misc

PAUSE
