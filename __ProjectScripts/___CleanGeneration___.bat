@echo off

cd ..

del VanaheimEngine.sln
del premake5.lua.bak
del Dependencies.lua.bak

rmdir /s /Q .vs
rmdir /s /Q bin
rmdir /s /Q bin-int

cd VanaheimEngine
del VanaheimEngine.vcxproj
del VanaheimEngine.vcxproj.user
cd ..

cd Game
del Game.vcxproj
del Game.vcxproj.user
cd ..

PAUSE
