add_executable(snake
	snake/main.cpp
)

target_compile_features(snake PRIVATE cxx_std_17)
target_link_libraries(snake
	PRIVATE
		CONAN_PKG::sfml
)

if(MSVC)
	target_compile_options(snake PRIVATE /W4 /WX /permissive-)
else()
	target_compile_options(snake PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
