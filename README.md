# KotonoEngine

## Build

### Visual Studio
- To build the solution for Visual Studio, execute *[build_vs17.sh](build_vs17.sh)* for Visual Studio 2022, or *[build_vs18.sh](build_vs18.sh)* for Visual Studio 2026.

- All generated build files will be located in *build/*.

### Other IDE
- To build the solution for other IDEs, create a build folder and run CMake for your IDE with the option :

    - -DWITH_EDITOR=ON for the solution with the editor.

    - -DWITH_EDITOR=OFF for the solution without the editor.

## Shader Compilation
To enable automatic shader compilation, make sure you have the [Vulkan SDK](https://vulkan.lunarg.com/sdk/home) and that its bin folder is added to your system’s PATH environment variable.
