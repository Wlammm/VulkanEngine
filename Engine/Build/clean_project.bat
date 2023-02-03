cd ..
del /s *.vcxproj
del /s *.vcxproj.user
del /s *.vcxproj.filters
del /s *.sln
rmdir /s /q .vs
cd ..
cd Bin/
del /s *.exe
del /s *.pdb
cd ..
rmdir /s /q Temp
pause