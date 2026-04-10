@echo off
setlocal

set "ROOT_DIR=%~dp0"
set "DEVKITARM=C:\devkitPro\devkitARM"
set "BUILD_DIR=%ROOT_DIR%build\3ds"
set "CMAKE_EXE=C:\Program Files\CMake\bin\cmake.exe"
set "SNAPSHOT_FILE=%BUILD_DIR%\.source_glob_snapshot.txt"
set "CURRENT_SNAPSHOT_FILE=%BUILD_DIR%\.source_glob_snapshot.current.txt"
set "NEED_CONFIGURE=0"

if not exist "%BUILD_DIR%" (
  mkdir "%BUILD_DIR%"
)

powershell -NoProfile -ExecutionPolicy Bypass -Command "$ErrorActionPreference='Stop'; $root = [System.IO.Path]::GetFullPath('%ROOT_DIR%'); $paths = @('Minecraft.Client','Minecraft.World','Minecraft.Server','include','cmake') | ForEach-Object { Join-Path $root $_ } | Where-Object { Test-Path $_ }; $patterns = @('*.c','*.cc','*.cpp','*.cxx','*.h','*.hh','*.hpp','*.inl','*.inc','*.asm','*.rc','*.cmake'); Get-ChildItem -Path $paths -Recurse -File -Include $patterns | ForEach-Object { $_.FullName.Substring($root.Length).TrimStart('\\').Replace('\\','/') } | Sort-Object -Unique | Set-Content -LiteralPath '%CURRENT_SNAPSHOT_FILE%' -Encoding ascii"
if errorlevel 1 exit /b 1

if not exist "%BUILD_DIR%\CMakeCache.txt" (
  set "NEED_CONFIGURE=1"
) else if exist "%SNAPSHOT_FILE%" (
  powershell -NoProfile -ExecutionPolicy Bypass -Command "$ErrorActionPreference='Stop'; $prev = Get-Content -LiteralPath '%SNAPSHOT_FILE%'; $curr = Get-Content -LiteralPath '%CURRENT_SNAPSHOT_FILE%'; $delta = Compare-Object -ReferenceObject $prev -DifferenceObject $curr | Where-Object { $_.SideIndicator -eq '=>' -or $_.SideIndicator -eq '<=' }; if ($delta) { exit 3 } else { exit 0 }"
  if errorlevel 4 exit /b 1
  if errorlevel 3 set "NEED_CONFIGURE=1"
)

if "%NEED_CONFIGURE%"=="1" (
  echo [3ds] Source file set changed or cache missing. Running configure...
  pushd "%ROOT_DIR%"
  "%CMAKE_EXE%" --preset 3ds
  if errorlevel 1 (
    popd
    exit /b 1
  )
  popd
)

if not exist "%SNAPSHOT_FILE%" (
  copy /y "%CURRENT_SNAPSHOT_FILE%" "%SNAPSHOT_FILE%" >nul
) else if "%NEED_CONFIGURE%"=="1" (
  copy /y "%CURRENT_SNAPSHOT_FILE%" "%SNAPSHOT_FILE%" >nul
)

pushd "%ROOT_DIR%"
"%CMAKE_EXE%" --build "%BUILD_DIR%" --config Debug -- -j 1
set "EXIT_CODE=%ERRORLEVEL%"
popd

exit /b %EXIT_CODE%