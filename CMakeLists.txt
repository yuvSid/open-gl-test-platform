cmake_minimum_required(VERSION 3.12)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project(openGLTestPlatform)

# generate list of files to build from
file(GLOB openGLTEstPlatform_SRC CONFIGURE_DEPENDS "src/*.cpp")

# adds external libs and dependencies
add_subdirectory("./../libraries/glfw" "${PROJECT_BINARY_DIR}/glfw")
list(APPEND EXTRA_LIBS glfw)
list(APPEND EXTRA_INCLUDES "./../libraries/glad" 
                            "./../libraries/glfw/include" 
                            "./../libraries/glm" 
                            "./../libraries/stb")

# add the executable
add_executable(openGLTestPlatform ${openGLTEstPlatform_SRC} ./../libraries/glad/glad.c)

# links build dependencies
target_link_libraries(openGLTestPlatform PUBLIC ${EXTRA_LIBS})
target_include_directories(openGLTestPlatform PUBLIC "${PROJECT_BINARY_DIR}" ${EXTRA_INCLUDES})

# copy recource folder to build destination
if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${PROJECT_BINARY_DIR})
endif()

# visual studio settings
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT openGLTestPlatform)