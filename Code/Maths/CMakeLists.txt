cmake_minimum_required (VERSION 3.10)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS True)

set(SOURCE_LIST
	# TODO: make this optional
	"Tests/MatrixTest.cpp"

	"Maths.hpp"
	"MathsException.hpp"
	"MathsUtils.cpp"
	"MathsUtils.hpp"
	"Matrix.cpp"
	"Matrix.hpp"
	"Quaternion.cpp"
	"Quaternion.hpp"
	"Vector.cpp"
	"Vector.hpp"
)

add_library(sml SHARED ${SOURCE_LIST})

target_include_directories(sml PRIVATE "Maths")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})

set_target_properties(sml PROPERTIES PUBLIC_HEADER "Maths/Math.h")
set_target_properties(sml PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(sml PUBLIC cxx_std_20)