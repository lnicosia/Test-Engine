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
	"Inputs/Binding.cpp"
	"Inputs/SDLEvents.cpp"
	"Inputs/SDLEvents.hpp"
	"Inputs/Binding.hpp"
	"Inputs/Action.hpp"
	"maths/math.hpp" 
	"Rendering/SoftwareRendering/Raycaster.hpp"
	"Rendering/Renderer.hpp"
	"Rendering/SoftwareRenderer.hpp"
	"Rendering/Window.hpp"
	"Rendering/SDLWindow.hpp"
	"Rendering/GLRenderer.hpp"
	"Rendering/VulkanRenderer.hpp"
	"Log.hpp"
	"Inputs/Events.hpp"
	"Inputs/Events.cpp"
	"Rendering/SoftwareRendering/Raycaster.cpp"
	"Rendering/SoftwareRenderer.cpp"
	"Rendering/Window.cpp"
	"Rendering/SDLWindow.cpp"
	"Rendering/Renderer.cpp"
	"Rendering/GLRenderer.cpp"
	"Rendering/VulkanRenderer.cpp"
	"Rendering/Camera.hpp"
	"Rendering/Camera.cpp"
	"Assets/Textures/Texture.hpp"
	"Assets/Textures/Texture.cpp"
	"Rendering/SoftwareRendering/DrawUtils.cpp"
	"Maths/math.cpp"
	"Rendering/FrameData.hpp"
	"Rendering/FrameData.cpp"
	"Rendering/DebugLevel.hpp"
	"Assets/Asset.hpp"
	"Assets/AssetManager.hpp"
	"Assets/Fonts/Font.hpp"
	"Assets/Fonts/Font.cpp"
	"Assets/Asset.cpp"
	"Assets/AssetManager.cpp"
	"Assets/CheckFileType.hpp"
	"Assets/CheckFileType.cpp" "Assets/Fonts/TTFFont.hpp" "Assets/Fonts/TTFFont.cpp" "Assets/Textures/SoftwareTexture.hpp" "Assets/Textures/SoftwareTexture.cpp" "Rendering/D3DRenderer.hpp" "Rendering/D3DRenderer.cpp" "Rendering/SDL.hpp" "Rendering/SDL.cpp" "Rendering/RendererType.hpp" "Rendering/WindowManager.hpp" "Log.cpp")

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
	PRIVATE "ThirdParty"
	PRIVATE "./"
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

	target_link_libraries(${PROJECT_NAME}
		"${CMAKE_CURRENT_SOURCE_DIR}/../binaries/ThirdParty/vulkan/vulkan-1.lib"
	)

elseif (UNIX)

	target_link_libraries(${PROJECT_NAME}
		"${CMAKE_CURRENT_SOURCE_DIR}/../binaries/ThirdParty/vulkan/libvulkan.so.1"
	)

endif ()


if (MSVC)
	add_compile_options(/std:c++2a)
endif(MSVC)

# TODO: Add tests and install targets if needed.
