#!/bin/sh

if [ $# -eq 0 ]
	then
	echo "Please give a shader file to compile"
	exit 1
fi
../../Binaries/ThirdParty/vulkan/glslc $1 -o $1.spv
