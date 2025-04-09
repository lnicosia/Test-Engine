cmake_minimum_required (VERSION 3.10)

project ("Test-Engine"
	VERSION 0.0.1
	DESCRIPTION "Test engine"
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
	#TODO: remake: Test-Engine.rc
	"main.cpp"
	"Inputs/Binding.cpp"
	"Inputs/SDLEvents.cpp"
	"Inputs/SDLEvents.hpp"
	"Inputs/Binding.hpp"
	"Inputs/Action.hpp"
	"maths/math_tmp.hpp" 
	"Rendering/SoftwareRendering/Raycaster.hpp"
	"Rendering/Renderer.hpp"
	"Rendering/SoftwareRenderer.hpp"
	"Rendering/Window.hpp"
	"Rendering/SDLWindow.hpp"
	"Rendering/GLRenderer.hpp"
	"Rendering/Vulkan/VulkanRenderer.hpp"
	"Rendering/Vulkan/VulkanRenderer.cpp"
	"Rendering/Vulkan/VulkanVertex.hpp"
	"Debug/Log.hpp"
	"Inputs/Events.hpp"
	"Inputs/Events.cpp"
	"Rendering/SoftwareRendering/Raycaster.cpp"
	"Rendering/SoftwareRenderer.cpp"
	"Rendering/Window.cpp"
	"Rendering/SDLWindow.cpp"
	"Rendering/Renderer.cpp"
	"Rendering/GLRenderer.cpp"
	"Rendering/Camera.hpp"
	"Rendering/Camera.cpp"
	"Assets/Textures/Texture.hpp"
	"Assets/Textures/Texture.cpp"
	"Rendering/SoftwareRendering/DrawUtils.cpp"
	"Maths/math_tmp.cpp"
	"Rendering/FrameStats.hpp"
	"Rendering/FrameStats.cpp"
	"Rendering/DebugLevel.hpp"
	"Assets/Asset.hpp"
	"Assets/AssetManager.hpp"
	"Assets/Fonts/Font.hpp"
	"Assets/Fonts/Font.cpp"
	"Assets/Asset.cpp"
	"Assets/AssetManager.cpp"
	"Assets/CheckFileType.hpp"
	"Assets/CheckFileType.cpp"
	"Assets/Fonts/TTFFont.hpp"
	"Assets/Fonts/TTFFont.cpp"
	"Assets/Textures/SoftwareTexture.hpp"
	"Assets/Textures/SoftwareTexture.cpp"
	"Rendering/D3DRenderer.hpp"
	"Rendering/D3DRenderer.cpp"
	"Rendering/SDL.hpp"
	"Rendering/SDL.cpp"
	"Rendering/RendererType.hpp"
	"Rendering/WindowManager.hpp"
	"Debug/Log.cpp"
	"Debug/ANSI.hpp"
	"Debug/Assert.hpp"
	"Debug/Exception.hpp"
	"Debug/Exception.cpp"
	"Platform.hpp"
	"Rendering/Shaders/Shader.hpp"
	"Rendering/Shaders/Shader.cpp"
	"Rendering/Vertex.hpp"
)

set(Vulkan_LIBRARY ../../Binaries/ThirdParty/vulkan)	
set(Vulkan_INCLUDE_DIR ../../Code/ThirdParty/vulkan)	
find_package(Vulkan REQUIRED)

add_subdirectory(Maths)

target_link_directories(${PROJECT_NAME}
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl3-build
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl_ttf-build
)

target_link_libraries(${PROJECT_NAME}
	SDL3::SDL3
	SDL3_ttf::SDL3_ttf
	sml
)

target_include_directories(${PROJECT_NAME}
	PRIVATE "ThirdParty"
	PRIVATE "./"
	PRIVATE "Maths"
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
	$<$<CONFIG:Debug>:>
	$<$<CONFIG:RenderingDebug>:-DENABLE_VALIDATION_LAYERS>
	$<$<CONFIG:Release>:-DNDEBUG>
)

#set_property(${PROJECT_NAME} tgt PROPERTY CXX_STANDARD 11)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E echo 
			"Config = $<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RenderingDebug>:RenderingDebug>"
)
if (WIN32)

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			#"${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl3-build/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/SDL3.dll"
			"${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl3-build/SDL3.dll"
			${CMAKE_CURRENT_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			#"${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl_ttf-build/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/SDL3_ttf.dll"
			"${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl_ttf-build/SDL3_ttf.dll"
			${CMAKE_CURRENT_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>)

	target_link_libraries(${PROJECT_NAME}
		"${CMAKE_CURRENT_SOURCE_DIR}/../Binaries/ThirdParty/Vulkan/1.3.239.0/vulkan-1.lib"
	)

elseif (UNIX)

	target_link_libraries(${PROJECT_NAME}
		"${CMAKE_CURRENT_SOURCE_DIR}/../Binaries/ThirdParty/Vulkan/1.3.239.0/libvulkan.so.1"
	)

endif ()


if (MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /Zc:preprocessor)
endif(MSVC)

# TODO: Add tests and install targets if needed.
