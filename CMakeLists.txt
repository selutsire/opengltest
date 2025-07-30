cmake_minimum_required(VERSION 3.10)
project(opengltest)

find_package(opengl REQUIRED)
find_package(glfw3 REQUIRED)

add_library(glad STATIC glad.c)
target_include_directories(glad PUBLIC $(CMAKE_SOURCE_DIR))

add_executable(opengltest
	opengltest/main.cpp
)

target_link_libraries(opengltest
	glad
	glfw
	OpenGL::GL
)
