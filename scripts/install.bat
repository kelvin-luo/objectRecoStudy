@echo off
setlocal
cd /d "%~dp0\.."
set "PROJECT_DIR=%~dp0..\.."
set "BUILD_DIR=%PROJECT_DIR%\build_msvc"
set "CMAKE_EXE=D:\win10\cmake-4.3.2-windows-x86_64\bin\cmake.exe"
if not exist "%CMAKE_EXE%" set "CMAKE_EXE=cmake"

set "PREFIX=%PROJECT_DIR%\install"
"%CMAKE_EXE%" --install "%BUILD_DIR%" --prefix "%PREFIX%"
echo Installed to %PREFIX%
endlocal
