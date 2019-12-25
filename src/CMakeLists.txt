set(SOURCE_FILES
	snake/main.cpp
)

if(MSVC)
	add_executable(snake
		WIN32
		${SOURCE_FILES}
	)
else()
	add_executable(snake
		${SOURCE_FILES}
	)
endif()

target_compile_features(snake PRIVATE cxx_std_17)
target_link_libraries(snake
	PRIVATE
		CONAN_PKG::sfml
		CONAN_PKG::gsl_microsoft
		CONAN_PKG::whereami
)

if(MSVC)
	target_compile_options(snake PRIVATE /W4 /permissive-)
else()
	target_compile_options(snake PRIVATE -Wall -Wextra -pedantic)
endif()

add_custom_command(
	TARGET
		snake
	POST_BUILD
	COMMAND
		${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/fonts
		$<TARGET_FILE_DIR:snake>/fonts
)

install(
	TARGETS
		snake
	RUNTIME DESTINATION
		.
)

install(
	FILES
		${CMAKE_SOURCE_DIR}/fonts/PressStart2P/PressStart2P-Regular.ttf
	DESTINATION
		fonts/PressStart2P
)

install(
	FILES
		${CMAKE_SOURCE_DIR}/fonts/PressStart2P/OFL.txt
	DESTINATION
		licenses/PressStart2P
)

install(
	DIRECTORY
		${CMAKE_BINARY_DIR}/licenses
	DESTINATION
		.
)

install(
	FILES
		${CMAKE_SOURCE_DIR}/LICENSE.md
	DESTINATION
		licenses/snake
)
