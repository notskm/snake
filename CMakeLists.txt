cmake_minimum_required(VERSION 3.11)

project(snake)

include("${CMAKE_SOURCE_DIR}/cmake/conan.cmake")
conan_cmake_run(
	CONANFILE
		conanfile.txt
	BASIC_SETUP CMAKE_TARGETS
	BUILD
		missing
)

add_subdirectory(src)
