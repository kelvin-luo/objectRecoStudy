@echo off
setlocal
cd /d "%~dp0\.."
set "PROJECT_DIR=%~dp0..\.."
set "EXE=%PROJECT_DIR%\msvc_release\objectRecoStudy.exe"
if not exist "%EXE%" (
  echo Executable not found. Run scripts\build.bat first.
  exit /b 1
)
cd /d "%PROJECT_DIR%\msvc_release"
start "" "%EXE%"
endlocal
