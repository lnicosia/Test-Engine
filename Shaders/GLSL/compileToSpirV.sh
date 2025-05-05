#!/bin/sh

if [ $# -eq 0 ]
	then
	echo "Please give a shader file to compile"
	exit 1
fi
../../Binaries/ThirdParty/vulkan/1.3.239.0/glslc $1 -o ../SPIR-V/$1.spv
