@echo off
setlocal

set "ROOT_DIR=%~dp0"
set "BUILD_DIR=%ROOT_DIR%build\3ds"

if exist "%BUILD_DIR%" (
  rmdir /s /q "%BUILD_DIR%"
)

exit /b 0