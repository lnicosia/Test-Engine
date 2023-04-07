# CMakeList.txt : CMake project for Code, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)


project ("Test-Engine"
	VERSION 0.1.0
	HOMEPAGE_URL "https://github.com/lnicosia/Test-Engine"
)

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
	"../Code/main.cpp"
	"../Code/Inputs/Binding.cpp"
	"../Code/Inputs/SDLEvents.cpp"
	"../Code/Inputs/SDLEvents.hpp"
	"../Code/Inputs/Binding.hpp"
	"../Code/Inputs/Action.hpp"
	"../Code/maths/math.hpp" 
	"../Code/Rendering/SoftwareRendering/Raycaster.hpp"
	"../Code/Rendering/Renderer.hpp"
	"../Code/Rendering/SoftwareRenderer.hpp"
	"../Code/Rendering/Window.hpp"
	"../Code/Rendering/SDLWindow.hpp"
	"../Code/Rendering/GLRenderer.hpp"
	"../Code/Rendering/VulkanRenderer.hpp"
	"../Code/Debug/Log.hpp"
	"../Code/Inputs/Events.hpp"
	"../Code/Inputs/Events.cpp"
	"../Code/Rendering/SoftwareRendering/Raycaster.cpp"
	"../Code/Rendering/SoftwareRenderer.cpp"
	"../Code/Rendering/Window.cpp"
	"../Code/Rendering/SDLWindow.cpp"
	"../Code/Rendering/Renderer.cpp"
	"../Code/Rendering/GLRenderer.cpp"
	"../Code/Rendering/VulkanRenderer.cpp"
	"../Code/Rendering/Camera.hpp"
	"../Code/Rendering/Camera.cpp"
	"../Code/Assets/Textures/Texture.hpp"
	"../Code/Assets/Textures/Texture.cpp"
	"../Code/Rendering/SoftwareRendering/DrawUtils.cpp"
	"../Code/Maths/math.cpp"
	"../Code/Rendering/FrameData.hpp"
	"../Code/Rendering/FrameData.cpp"
	"../Code/Rendering/DebugLevel.hpp"
	"../Code/Assets/Asset.hpp"
	"../Code/Assets/AssetManager.hpp"
	"../Code/Assets/Fonts/Font.hpp"
	"../Code/Assets/Fonts/Font.cpp"
	"../Code/Assets/Asset.cpp"
	"../Code/Assets/AssetManager.cpp"
	"../Code/Assets/CheckFileType.hpp"
	"../Code/Assets/CheckFileType.cpp"
	"../Code/Assets/Fonts/TTFFont.hpp"
	"../Code/Assets/Fonts/TTFFont.cpp"
	"../Code/Assets/Textures/SoftwareTexture.hpp"
	"../Code/Assets/Textures/SoftwareTexture.cpp"
	"../Code/Rendering/D3DRenderer.hpp"
	"../Code/Rendering/D3DRenderer.cpp"
	"../Code/Rendering/SDL.hpp"
	"../Code/Rendering/SDL.cpp"
	"../Code/Rendering/RendererType.hpp"
	"../Code/Rendering/WindowManager.hpp"
	"../Code/Debug/Log.cpp"
	"../Code/Debug/ANSI.hpp"
	"../Code/Debug/Assert.hpp"
	"../Code/Debug/Exception.hpp"
	"../Code/Debug/Exception.cpp"
	"../Code/Platform.hpp")

find_package(Vulkan REQUIRED)

target_link_directories(${PROJECT_NAME}
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl3-build
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl_ttf-build
)

target_link_libraries(${PROJECT_NAME}
	SDL3::SDL3
	SDL3_ttf::SDL3_ttf
)

target_include_directories(${PROJECT_NAME}
	PRIVATE "../Code/ThirdParty"
	PRIVATE "./"
)

if (NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug")
	add_definitions("-DNDEBUG")
else()
endif()

message(STATUS "Config = ${CMAKE_BUILD_TYPE}")

#set_property(${PROJECT_NAME} tgt PROPERTY CXX_STANDARD 11)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl3-build/SDL3.dll"
			${CMAKE_CURRENT_BINARY_DIR})
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl_ttf-build/SDL3_ttf.dll"
			${CMAKE_CURRENT_BINARY_DIR})

	target_link_libraries(${PROJECT_NAME}
		"${CMAKE_CURRENT_SOURCE_DIR}/../Binaries/ThirdParty/vulkan/vulkan-1.lib"
	)

elseif (UNIX)

	target_link_libraries(${PROJECT_NAME}
		"${CMAKE_CURRENT_SOURCE_DIR}/../Binaries/ThirdParty/vulkan/libvulkan.so.1"
	)

endif ()


if (MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /Zc:preprocessor)
endif(MSVC)

# TODO: Add tests and install targets if needed.
