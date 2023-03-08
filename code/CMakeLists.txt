# CMakeList.txt : CMake project for code, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.11)

include(FetchContent)

FetchContent_Declare(
	SDL3
	GIT_REPOSITORY	https://github.com/libsdl-org/SDL
	GIT_TAG			53bde3abee6bc64c3f5368c37e1b685ac18aafb3
)
FetchContent_MakeAvailable(SDL3)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl3-build)
set(SDL3_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl3-build)
set(SDL3_INCLUDE_DIR
	${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl3-src/include
	${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl3-src/include/SDL3
)
if (WIN32)
	set(SDL3_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl3-build/libSDL3.dll)
endif (WIN32)

if (UNIX)
	set(SDL3_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl3-build/libSDL3.so)
endif (UNIX)


#find_file(SDL3_INCLUDE_DIR NAME SDL.h HINTS SDL3)
#find_library(SDL3_LIBRARY NAME SDL3)

project ("Test-Engine")

# Add source to this project's executable.
#find_package(SDL3)
include_directories(${SDL3_INCLUDE_DIR})
add_executable (${PROJECT_NAME} "main.cpp" )

#target_include_directories(${PROJECT_NAME} ${SDL3_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDL3_LIBRARY})

# TODO: Add tests and install targets if needed.
