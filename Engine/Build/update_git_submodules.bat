@echo off
cd ..
cd ..
echo Updating submodules from github...

git submodule init
git submodule update
echo Update successful
PAUSE
