@echo off
setlocal

set "ROOT_DIR=%~dp0"
set "DEVKITARM=C:\devkitPro\devkitARM"
set "CMAKE_EXE=C:\Program Files\CMake\bin\cmake.exe"

pushd "%ROOT_DIR%"
"%CMAKE_EXE%" --preset 3ds --fresh
set "EXIT_CODE=%ERRORLEVEL%"
popd

exit /b %EXIT_CODE%