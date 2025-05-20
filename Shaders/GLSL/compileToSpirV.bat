@echo off
if "%~1"=="" (
    echo Please give a shader file to compile
    exit /b 1
)
if not exist "..\SPIR-V" (
    mkdir "..\SPIR-V"
)
:loop
if "%~1"=="" goto :eof
echo Compiling %1 to ..\SPIR-V\%~nx1.spv
glslc "%~1" -o "..\SPIR-V\%~nx1.spv"
shift
goto loop
