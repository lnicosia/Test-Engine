@echo off
IF "%1"=="" (
	ECHO Please give a shader file to compile
	EXIT /B
)

..\..\Binaries\ThirdParty\vulkan\glslc.exe %1 -o %1.spv
