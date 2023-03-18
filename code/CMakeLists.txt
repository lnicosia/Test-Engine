# CMakeList.txt : CMake project for code, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

project ("Test-Engine")

include(FetchContent)

FetchContent_Declare(
	SDL3
	GIT_REPOSITORY	https://github.com/libsdl-org/SDL
	GIT_TAG			53bde3abee6bc64c3f5368c37e1b685ac18aafb3
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
	SDL_ttf
	GIT_REPOSITORY	https://github.com/libsdl-org/SDL_ttf
	GIT_TAG			1659723b95254600ba044964c8e23fd3db4b9bfd
)
FetchContent_MakeAvailable(SDL_ttf)


# Add source to this project's executable.

add_executable (${PROJECT_NAME}
	"main.cpp"
	"Inputs/Binding.class.cpp"
	"Inputs/SDLEvents.class.cpp"
	"Inputs/SDLEvents.class.hpp"
	"Inputs/Binding.class.hpp"
	"Inputs/Action.class.hpp"
	"Inputs/math.hpp")


target_link_directories(${PROJECT_NAME}
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl3-build
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl_ttf-build
)

target_link_libraries(${PROJECT_NAME}
	SDL3::SDL3
	SDL3_ttf::SDL3_ttf
)

target_include_directories(${PROJECT_NAME}
	PRIVATE "external"
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

if (WIN32)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl3-build/SDL3.dll"
			${CMAKE_CURRENT_BINARY_DIR})
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl_ttf-build/SDL3_ttf.dll"
			${CMAKE_CURRENT_BINARY_DIR})
endif (WIN32)

if (MSVC)
	add_compile_options(/std:c++2a)
endif(MSVC)

# TODO: Add tests and install targets if needed.
