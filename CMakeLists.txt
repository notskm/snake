cmake_minimum_required(VERSION 3.11)

project(snake)

include("${CMAKE_SOURCE_DIR}/cmake/conan.cmake")

conan_check()
conan_add_remote(
	NAME
		bincrafters
	URL
		https://api.bintray.com/conan/bincrafters/public-conan
)

conan_cmake_run(
	CONANFILE
		conanfile.txt
	BASIC_SETUP CMAKE_TARGETS
	BUILD
		missing
)

add_subdirectory(src)
