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

FetchContent_Declare(
	assimp
	GIT_REPOSITORY	https://github.com/assimp/assimp
	GIT_TAG			c35200e
)
FetchContent_MakeAvailable(assimp)


# Add source to this project's executable.

add_executable (${PROJECT_NAME}
	#TODO: remake: Test-Engine.rc
	"main.cpp"
	"Platform.hpp"

	"Assets/Fonts/Font.cpp"
	"Assets/Fonts/Font.hpp"

	"Assets/Importers/Assimp/AssimpHelpers.cpp"
	"Assets/Importers/Assimp/AssimpHelpers.hpp"
	"Assets/Importers/Assimp/AssimpImporter.cpp"
	"Assets/Importers/Assimp/AssimpImporter.hpp"
	"Assets/Importers/ObjectImporter.cpp"
	"Assets/Importers/ObjectImporter.hpp"

	"Assets/Fonts/TTFFont.cpp"
	"Assets/Fonts/TTFFont.hpp"

	"Assets/Textures/SoftwareTexture.cpp"
	"Assets/Textures/SoftwareTexture.hpp"
	"Assets/Textures/Texture.cpp"
	"Assets/Textures/Texture.hpp"
	"Assets/Textures/VulkanTexture.cpp"
	"Assets/Textures/VulkanTexture.hpp"

	"Assets/Asset.cpp"
	"Assets/Asset.hpp"
	"Assets/AssetManager.cpp"
	"Assets/AssetManager.hpp"
	"Assets/CheckFileType.cpp"
	"Assets/CheckFileType.hpp"

	"Debug/ANSI.hpp"
	"Debug/Assert.hpp"
	"Debug/Exception.cpp"
	"Debug/Exception.hpp"
	"Debug/Log.cpp"
	"Debug/Log.hpp"

	"Inputs/Action.hpp"
	"Inputs/Binding.cpp"
	"Inputs/Binding.hpp"
	"Inputs/Events.cpp"
	"Inputs/Events.hpp"
	"Inputs/SDLEvents.cpp"
	"Inputs/SDLEvents.hpp"

	"Maths/math_tmp.cpp"
	"Maths/math_tmp.hpp" 

	"Rendering/D3D/D3DRenderer.cpp"
	"Rendering/D3D/D3DRenderer.hpp"

	"Rendering/OpenGL/GLRenderer.cpp"
	"Rendering/OpenGL/GLRenderer.hpp"

	"Rendering/Shaders/Shader.cpp"
	"Rendering/Shaders/Shader.hpp"

	"Rendering/Vulkan/DescriptorSets/VulkanDescriptorAllocator.cpp"
	"Rendering/Vulkan/DescriptorSets/VulkanDescriptorAllocator.hpp"
	"Rendering/Vulkan/DescriptorSets/VulkanDescriptorWriter.cpp"
	"Rendering/Vulkan/DescriptorSets/VulkanDescriptorWriter.hpp"
	"Rendering/Vulkan/QueueFamilyIndices.hpp"
	"Rendering/Vulkan/VulkanDevice.cpp"
	"Rendering/Vulkan/VulkanDevice.hpp"
	"Rendering/Vulkan/VulkanMesh.cpp"
	"Rendering/Vulkan/VulkanMesh.hpp"
	"Rendering/Vulkan/VulkanRenderer.cpp"
	"Rendering/Vulkan/VulkanRenderer.hpp"
	"Rendering/Vulkan/VulkanUtils.cpp"
	"Rendering/Vulkan/VulkanUtils.hpp"
	"Rendering/Vulkan/VulkanVertex.hpp"

	"Rendering/Window/SDL.cpp"
	"Rendering/Window/SDL.hpp"
	"Rendering/Window/SDLWindow.cpp"
	"Rendering/Window/SDLWindow.hpp"
	"Rendering/Window/Window.cpp"
	"Rendering/Window/Window.hpp"
	"Rendering/Window/WindowAPI.hpp"

	"Rendering/SoftwareRendering/DrawUtils.cpp"
	"Rendering/SoftwareRendering/DrawUtils.hpp"
	"Rendering/SoftwareRendering/Raycaster.cpp"
	"Rendering/SoftwareRendering/Raycaster.hpp"
	"Rendering/SoftwareRendering/SoftwareRenderer.cpp"
	"Rendering/SoftwareRendering/SoftwareRenderer.hpp"
	
	"Rendering/Camera.cpp"
	"Rendering/Camera.hpp"
	"Rendering/DebugLevel.hpp"
	"Rendering/FrameStats.cpp"
	"Rendering/FrameStats.hpp"
	"Rendering/GPUDevice.cpp"
	"Rendering/GPUDevice.hpp"
	"Rendering/MeshGeometry.cpp"
	"Rendering/MeshGeometry.hpp"
	"Rendering/MeshInternal.cpp"
	"Rendering/MeshInternal.hpp"
	"Rendering/Renderer.cpp"
	"Rendering/Renderer.hpp"
	"Rendering/RendererType.hpp"
	"Rendering/Vertex.hpp"

	"Scene/Scene.cpp"
	"Scene/Scene.hpp"
	"Scene/Actor.cpp"
	"Scene/Actor.hpp"
	"Scene/Component.cpp"
	"Scene/Component.hpp"
	"Scene/Transform.hpp"
)

set(Vulkan_LIBRARY ../../Binaries/ThirdParty/vulkan)	
set(Vulkan_INCLUDE_DIR ../../Code/ThirdParty/vulkan)	
find_package(Vulkan REQUIRED)

add_subdirectory(Maths)

target_link_directories(${PROJECT_NAME}
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl3-build
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl_ttf-build
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/_deps/assimp-build
)

target_link_libraries(${PROJECT_NAME} PUBLIC
	SDL3::SDL3
	SDL3_ttf::SDL3_ttf
)
target_link_libraries(${PROJECT_NAME} PUBLIC
	assimp
)
target_link_libraries(${PROJECT_NAME} PUBLIC
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
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E echo 
			"Config = $<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RenderingDebug>:RenderingDebug>"
)
if (WIN32)

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			#"${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl3-build/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/SDL3.dll"
			"${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl3-build/SDL3.dll"
			${CMAKE_CURRENT_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>
	)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			#"${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl_ttf-build/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/SDL3_ttf.dll"
			"${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl_ttf-build/SDL3_ttf.dll"
			${CMAKE_CURRENT_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>
	)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${CMAKE_CURRENT_BINARY_DIR}/_deps/assimp-build/bin/assimp-vc143-mt.dll"
			${CMAKE_CURRENT_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>
	)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${CMAKE_CURRENT_BINARY_DIR}/Maths/sml.dll"
			${CMAKE_CURRENT_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>
	)

	target_link_libraries(${PROJECT_NAME} PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/../Binaries/ThirdParty/Vulkan/1.3.239.0/vulkan-1.lib"
	)

elseif (UNIX)

	target_link_libraries(${PROJECT_NAME} PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/../Binaries/ThirdParty/Vulkan/1.3.239.0/libvulkan.so.1"
	)

endif ()


if (MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /Zc:preprocessor)
endif(MSVC)

# TODO: Add tests and install targets if needed.
