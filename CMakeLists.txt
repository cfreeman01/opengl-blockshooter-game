cmake_minimum_required(VERSION 3.2)
project(Game VERSION 1.1)

if(UNIX)
  set(OpenGL_GL_PREFERENCE LEGACY)
endif(UNIX)

find_package(OpenGL REQUIRED)

#copy resources to build directory
file(COPY shaders textures audio DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#collect and add source files
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*)
add_executable(${PROJECT_NAME} ${SRC_FILES})

#includes
include_directories(${PROJECT_NAME} PUBLIC include)

#build and link GLFW
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glfw-3.3.4/)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw)