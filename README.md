# OpenGL Shooter 

A simple 2D shooter game made with C++ and OpenGL

![gameplay video](gameplay.gif)
  ## Features
  - :trophy: Award-winning graphics and sound design
  - :skull_and_crossbones: Several unique enemy types
  - :smiling_imp: Epic boss fights
  - :clock4: Endless gameplay with steadily increasing difficulty
  - :rainbow: Rainbow colors!
  ## Building and Running
  - Use CMake to generate build files, all dependencies are included
    - Unix-like systems may require additional packages for GLFW as described [here](https://www.glfw.org/docs/latest/compile_guide.html#compile_deps_x11)
  - Resource files (audio/textures/shaders) are copied to your build directory by CMake, so the program can find the files if it is run from the root or build directory
  - This program has been tested on:
     - Windows 10 (MSVC)
     - Ubuntu 20.04.3 (g++)
   ## External Libraries Used
   - [GLFW](https://github.com/glfw/glfw)
   - [GLAD](https://github.com/Dav1dde/glad)
   - [GLM](https://glm.g-truc.net/0.9.8/index.html)
   - [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)
   - [SoLoud](https://github.com/jarikomppa/soloud), [MiniAudio](https://github.com/mackron/miniaudio)
   - Credit to [JoeyDeVries/LearnOpenGL](https://github.com/JoeyDeVries/LearnOpenGL) for the Texture, Shader, SpriteRenderer, and ResourceManager classes
