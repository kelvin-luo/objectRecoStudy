@echo off
setlocal EnableExtensions
cd /d "%~dp0\.."

set "PROJECT_DIR=%~dp0..\.."
set "CODE_DIR=%~dp0.."
set "BUILD_DIR=%PROJECT_DIR%\build_msvc"
set "CMAKE_EXE=D:\win10\cmake-4.3.2-windows-x86_64\bin\cmake.exe"
set "NINJA_EXE=D:\win10\ninja.exe"

if not exist "%CMAKE_EXE%" set "CMAKE_EXE=cmake"
if not exist "%NINJA_EXE%" set "NINJA_EXE=ninja"

set "CONFIG=Release"
if /I "%~1"=="Debug" set "CONFIG=Debug"

REM Activate MSVC (required for MFC). Adjust path if your VS edition differs.
set "VCVARS="
for %%E in (Community Professional Enterprise BuildTools) do (
  if exist "C:\Program Files\Microsoft Visual Studio\2022\%%E\VC\Auxiliary\Build\vcvars64.bat" (
    set "VCVARS=C:\Program Files\Microsoft Visual Studio\2022\%%E\VC\Auxiliary\Build\vcvars64.bat"
  )
)
if defined VCVARS (
  call "%VCVARS%" >nul
) else (
  echo WARNING: vcvars64.bat not found. Install VS2022 C++ / MFC workload.
)

echo [1/4] Generate icons...
py -3 "%CODE_DIR%\scripts\generate_icons.py" 2>nul
python "%CODE_DIR%\scripts\generate_icons.py" 2>nul

echo [2/4] Configure (%CONFIG%)...
set "OPENCV_CMAKE=D:/win10/opencv500/build"
if not exist "%OPENCV_CMAKE%\OpenCVConfig.cmake" set "OPENCV_CMAKE=D:/win10/opencv4130/build/x64/vc16/lib"

"%CMAKE_EXE%" -S "%CODE_DIR%" -B "%BUILD_DIR%" -G "Visual Studio 17 2022" -A x64 ^
  -DOPENCV_DIR=%OPENCV_CMAKE%
if errorlevel 1 exit /b 1

echo [3/4] Build...
"%CMAKE_EXE%" --build "%BUILD_DIR%" --config %CONFIG%
if errorlevel 1 exit /b 1

echo [4/4] Sync input data...
if not exist "%PROJECT_DIR%\msvc_release\input" mkdir "%PROJECT_DIR%\msvc_release\input"
if exist "D:\k8\media_images\xi_an_hot\people.jpeg" (
  copy /Y "D:\k8\media_images\xi_an_hot\people.jpeg" "%PROJECT_DIR%\msvc_release\input\people.jpeg" >nul
)
if exist "D:\k8\media_260612\2026-06-15_154048_036.mp4" (
  copy /Y "D:\k8\media_260612\2026-06-15_154048_036.mp4" "%PROJECT_DIR%\msvc_release\input\demo.mp4" >nul
)

echo Build OK: %PROJECT_DIR%\msvc_release\objectRecoStudy.exe
endlocal
