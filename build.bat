@echo off
setlocal enabledelayedexpansion

:: Build directory
set BUILD_DIR=build

:: Create build directory if missing
if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
)

cd "%BUILD_DIR%"

:: Configure CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

:: Detect CPU cores (fallback to 1 if undefined)
set CORES=%NUMBER_OF_PROCESSORS%
if "%CORES%"=="" set CORES=1

:: Build with parallel jobs depending on generator
if exist "build.ninja" (
    cmake --build . -- -j%CORES%
) else (
    cmake --build . --config Release -- /m:%CORES%
)

cd ..
endlocal
