cmake_minimum_required (VERSION 3.10)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS True)

add_library(sml SHARED
	"Vector.hpp"
	"Vector.cpp"
)

target_include_directories(sml PRIVATE Maths)

set_target_properties(sml PROPERTIES PUBLIC_HEADER "Maths/Math.h")
set_target_properties(sml PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(sml PUBLIC cxx_std_20)