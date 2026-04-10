@echo off
setlocal

set "ROOT_DIR=%~dp0"
if defined DEVKITPRO (
	set "DEVKITARM=%DEVKITPRO%\devkitARM"
) else (
	set "DEVKITPRO=C:\devkitPro"
	set "DEVKITARM=%DEVKITPRO%\devkitARM"
)
set "CMAKE_EXE=C:\Program Files\CMake\bin\cmake.exe"

pushd "%ROOT_DIR%"
"%CMAKE_EXE%" --preset 3ds --fresh
set "EXIT_CODE=%ERRORLEVEL%"
popd

exit /b %EXIT_CODE%