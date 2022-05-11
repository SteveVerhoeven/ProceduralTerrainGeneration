pushd ..\
call Libs\Premake\premake5.exe vs2019

start VanaheimEngine.sln
popd

PAUSE