cmake_minimum_required(VERSION 3.2)
project(Game VERSION 1.0)

find_package(OpenGL REQUIRED)

#copy resources to build directory
file(COPY shaders textures audio DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME}
               src/audioPlayer.h
               src/blockObject.cpp
               src/blockObject.h
               src/character.cpp
               src/character.h
               src/enemy.cpp
               src/enemy.h
               src/enemy1.cpp
               src/enemy1.h
               src/enemy2.cpp
               src/enemy2.h
               src/enemy3.cpp
               src/enemy3.h
               src/enemy3_mask.cpp
               src/enemy3_mask.h
               src/game.cpp
               src/game.h
               src/gameObject.cpp
               src/gameObject.h
               src/glad.c
               src/healthBlock.cpp
               src/healthBlock.h
               src/level.cpp
               src/level.h
               src/main.cpp
               src/monkeyBoss.cpp
               src/monkeyBoss.h
               src/playerObject.cpp
               src/playerObject.h
               src/resource_manager.cpp
               src/resource_manager.h
               src/shader.cpp
               src/shader.h
               src/spriteRenderer.cpp
               src/spriteRenderer.h
               src/starBlock.cpp
               src/starBlock.h
               src/texture.cpp
               src/texture.h
               src/trailGenerator.cpp
               src/trailGenerator.h)

include_directories(${PROJECT_NAME} PUBLIC includes)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glfw-3.3.4/)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
