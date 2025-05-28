cmake_minimum_required (VERSION 3.10)

set(SOURCE_LIST
	"main.cpp"
	"ArenaTest.cpp"
	"ArenaTest.hpp"
)
add_executable(UnitTests ${SOURCE_LIST})

target_link_libraries(UnitTests PUBLIC Test-Engine)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})

if (WIN32)

	add_custom_command(TARGET UnitTests POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:SDL3::SDL3> ${CMAKE_CURRENT_BINARY_DIR}
			$<TARGET_FILE:SDL3_ttf::SDL3_ttf> ${CMAKE_CURRENT_BINARY_DIR}
			$<TARGET_FILE:assimp> ${CMAKE_CURRENT_BINARY_DIR}
			$<TARGET_FILE:sml> ${CMAKE_CURRENT_BINARY_DIR}
			$<TARGET_FILE:Test-Engine> ${CMAKE_CURRENT_BINARY_DIR}
	)

endif ()

if (MSVC)
	target_compile_options(UnitTests PRIVATE /Zc:preprocessor)
endif(MSVC)