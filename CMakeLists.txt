cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project(openGLTestPlatform)

include_directories(./../libraries/glad ./../libraries/glfw/include ./../libraries/glm ./../libraries/stb)
# add the MathFunctions library
# add_subdirectory(./../libraries/glad subproject/glad)

# add the executable
add_executable(openGLTestPlatform ./src/main.cpp)