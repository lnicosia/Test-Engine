# Building

## Requirements

* CMake 3.10 minimum
* C++ 20 compiler
* gcc 10 minimum

## Tested builds

* Ubuntu 20.04 WSL
* Windows 10 in VSCode

## How to build

Run `cmake -S . -B Build/Linux` or `cmake -S . -B Build/Windows` to configure the project

Run `cmake --build Build/Linux` or `cmake --build Build/Windows` to build the project
