cmake_minimum_required(VERSION 3.11)

project(snake)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/develop/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

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
