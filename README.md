# blahajEngine
A work in progress game engine made using C++ and Vulkan.

Supports Linux & Windows

## Build Instructions
You will need the following packages for building on Arch Linux
- base-devel
- vulkan-devel
- glfw
- glm
- stb
- lua

You will need the following packages for building on Windows using msys2/mingw64 (You may also want the LunarG Vulkan SDK)
- base-devel
- mingw-w64-x86_64-toolchain
- mingw-w64-x86_64-vulkan-devel
- mingw-w64-x86_64-shaderc
- mingw-w64-x86_64-glfw
- mingw-w64-x86_64-glm
- mingw-w64-x86_64-stb
- mingw-w64-x86_64-lua

With the correct packages installed you can just run `make` with the included Makefile to compile the program and the shaders
