cmake_minimum_required(VERSION 3.15)

project(cdaw
	VERSION 1.0
	DESCRIPTION "A DAW written in C"
	LANGUAGES C
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
	src/main.c
	src/platform/linux/linux_gl_context.c
	src/core/str.c
	src/core/arena.c
)

find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_compile_options(${PROJECT_NAME} PRIVATE
	-Wall
	-Werror
	-Wextra
	-Wpedantic
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${X11_LIBRARIES} OpenGL::GL)
