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

set(EXEC_NAME ${PROJECT_NAME}-App)

add_executable (${EXEC_NAME}
	"main.cpp"
)

target_include_directories(${EXEC_NAME}
	PRIVATE "ThirdParty"
	PRIVATE "./"
	PRIVATE "Maths"
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${EXEC_NAME})
target_link_libraries(${EXEC_NAME} PUBLIC ${PROJECT_NAME})

set(SOURCE_LIST
	#TODO: remake: Test-Engine.rc
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

	"Misc/Arena.cpp"
	"Misc/Arena.hpp"

	"Rendering/D3D/D3DRenderer.cpp"
	"Rendering/D3D/D3DRenderer.hpp"

	"Rendering/Materials/Material.cpp"
	"Rendering/Materials/Material.hpp"

	"Rendering/OpenGL/GLRenderer.cpp"
	"Rendering/OpenGL/GLRenderer.hpp"

	"Rendering/Shaders/Shader.cpp"
	"Rendering/Shaders/Shader.hpp"

	"Rendering/SoftwareRendering/DrawUtils.cpp"
	"Rendering/SoftwareRendering/DrawUtils.hpp"
	"Rendering/SoftwareRendering/Raycaster.cpp"
	"Rendering/SoftwareRendering/Raycaster.hpp"
	"Rendering/SoftwareRendering/SoftwareMesh.cpp"
	"Rendering/SoftwareRendering/SoftwareMesh.hpp"
	"Rendering/SoftwareRendering/SoftwareRenderer.cpp"
	"Rendering/SoftwareRendering/SoftwareRenderer.hpp"

	"Rendering/Vulkan/DescriptorSets/VulkanDescriptorAllocator.cpp"
	"Rendering/Vulkan/DescriptorSets/VulkanDescriptorAllocator.hpp"
	"Rendering/Vulkan/DescriptorSets/VulkanDescriptorWriter.cpp"
	"Rendering/Vulkan/DescriptorSets/VulkanDescriptorWriter.hpp"
	"Rendering/Vulkan/QueueFamilyIndices.hpp"
	"Rendering/Vulkan/VulkanDevice.cpp"
	"Rendering/Vulkan/VulkanDevice.hpp"
	"Rendering/Vulkan/VulkanMaterial.cpp"
	"Rendering/Vulkan/VulkanMaterial.hpp"
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
	
	"Rendering/Camera.cpp"
	"Rendering/Camera.hpp"
	"Rendering/DebugLevel.hpp"
	"Rendering/DeletionQueue.cpp"
	"Rendering/DeletionQueue.hpp"
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

	"Scene/Actor.cpp"
	"Scene/Actor.hpp"
	"Scene/Component.cpp"
	"Scene/Component.hpp"
	"Scene/Entity.hpp"
	"Scene/Scene.cpp"
	"Scene/Scene.hpp"
	"Scene/Transform.hpp"
)

add_library (${PROJECT_NAME} SHARED ${SOURCE_LIST})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})

set_property(TARGET ${PROJECT_NAME} PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS 1)

set(Vulkan_LIBRARY ../../Binaries/ThirdParty/vulkan)
set(Vulkan_INCLUDE_DIR ../../Code/ThirdParty/vulkan)
find_package(Vulkan REQUIRED)

add_subdirectory(Maths)
add_subdirectory(Tests)

set(SDL_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl3-build)
set(SDL_TTF_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl_ttf-build)
set(ASSIMP_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/assimp-build)

target_link_directories(${PROJECT_NAME}
	PRIVATE ${SDL_BUILD_DIR}
	PRIVATE ${SDL_TTF_BUILD_DIR}
	PRIVATE ${ASSIMP_BUILD_DIR}
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
	PUBLIC "ThirdParty"
	PUBLIC "./"
	PUBLIC "Maths"
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
	$<$<CONFIG:Debug>:>
	$<$<CONFIG:RenderingDebug>:-DENABLE_VALIDATION_LAYERS>
	$<$<CONFIG:Release>:-DNDEBUG>
	-DTE_COMPILING_DLL
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

get_target_property(SDL3_OG SDL3::SDL3 ALIASED_TARGET)
get_target_property(SDL3_TTF_OG SDL3_ttf::SDL3_ttf ALIASED_TARGET)

get_property(OUTS DIRECTORY "${ASSIMP_BUILD_DIR}" PROPERTY IMPORT_TARGETS)
foreach(OUT IN LISTS OUTS)
	message(STATUS "Assimp links : ${OUT}")
endforeach()

function(create_target_tree DIR LIB)
    get_property(TGTS DIRECTORY "${DIR}" PROPERTY BUILDSYSTEM_TARGETS)
    foreach(TGT IN LISTS TGTS)
        #message(STATUS "Target: Lib/ThirdParty/${LIB}/${TGT}")
		set_target_properties(${TGT} PROPERTIES FOLDER "Lib/ThirdParty/${LIB}")
        # TODO: Do something about it
    endforeach()

    get_property(SUBDIRS DIRECTORY "${DIR}" PROPERTY SUBDIRECTORIES)
    foreach(SUBDIR IN LISTS SUBDIRS)
        create_target_tree("${SUBDIR}" ${LIB})
    endforeach()
endfunction()

create_target_tree("${ASSIMP_BUILD_DIR}" assimp)
create_target_tree("${SDL_BUILD_DIR}" ${SDL3_OG})
create_target_tree("${SDL_TTF_BUILD_DIR}" ${SDL3_TTF_OG})
set_target_properties(sml PROPERTIES FOLDER "Lib/")

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E echo 
			"Config = $<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RenderingDebug>:RenderingDebug>"
)
if (WIN32)

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:SDL3::SDL3> ${CMAKE_CURRENT_BINARY_DIR}
			$<TARGET_FILE:SDL3_ttf::SDL3_ttf> ${CMAKE_CURRENT_BINARY_DIR}
			$<TARGET_FILE:assimp> ${CMAKE_CURRENT_BINARY_DIR}
			$<TARGET_FILE:sml> ${CMAKE_CURRENT_BINARY_DIR}
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
