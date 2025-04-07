@echo off
IF "%1"=="" (
	ECHO Please give a shader file to compile
	EXIT /B
)

..\..\Binaries\ThirdParty\vulkan\1.3.239.0\glslc.exe %1 -o ..\SPIR-V\%1.spv
