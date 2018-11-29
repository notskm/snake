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
)

if(MSVC)
	target_compile_options(snake PRIVATE /W4 /WX /permissive-)
else()
	target_compile_options(snake PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
