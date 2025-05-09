cmake_minimum_required (VERSION 3.10)

set(SOURCE_LIST
	"main.cpp"
	"ArenaTest.cpp"
	"ArenaTest.hpp"
)
add_executable(Tests ${SOURCE_LIST})

target_link_libraries(Tests PUBLIC Test-Engine)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})

set_target_properties(sml PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(sml PUBLIC cxx_std_20)

if (WIN32)

	add_custom_command(TARGET Tests POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:SDL3::SDL3> ${CMAKE_CURRENT_BINARY_DIR}
			$<TARGET_FILE:SDL3_ttf::SDL3_ttf> ${CMAKE_CURRENT_BINARY_DIR}
			$<TARGET_FILE:assimp> ${CMAKE_CURRENT_BINARY_DIR}
			$<TARGET_FILE:sml> ${CMAKE_CURRENT_BINARY_DIR}
			$<TARGET_FILE:Test-Engine> ${CMAKE_CURRENT_BINARY_DIR}
	)

	target_link_libraries(Tests PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/../../Binaries/ThirdParty/Vulkan/1.3.239.0/vulkan-1.lib"
	)

elseif (UNIX)

	target_link_libraries(Tests PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/../../Binaries/ThirdParty/Vulkan/1.3.239.0/libvulkan.so.1"
	)

endif ()

if (MSVC)
	target_compile_options(Tests PRIVATE /Zc:preprocessor)
endif(MSVC)