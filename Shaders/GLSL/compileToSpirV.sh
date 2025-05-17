#!/bin/sh

if [ $# -eq 0 ]
	then
	echo "Please give a shader file to compile"
	exit 1
fi
mkdir -p ../SPIR-V
for file in "$@"
    do
        echo "Compiling $file to ../SPIR-V/$file.spv"
        glslc $file -o ../SPIR-V/$file.spv
    done
