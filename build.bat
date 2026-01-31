@echo off
echo Building Codesys-Webots Bridge...

if not exist build mkdir build

echo [1/3] Compiling Resources...
windres src/resource.rc -O coff -o build/resource.o
if %errorlevel% neq 0 exit /b %errorlevel%

echo [2/3] Compiling Application...
g++ -std=c++17 -Iinclude -o CodesysWebotsBridge.exe src/main.cpp build/resource.o
if %errorlevel% neq 0 exit /b %errorlevel%

echo [3/3] Build Complete! Run with: .\CodesysWebotsBridge.exe
