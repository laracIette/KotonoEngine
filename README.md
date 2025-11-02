# KotonoEngine

## Build

### Visual Studio 17
- To build the solution for Visual Studio 17, execute :

    - *[KotonoTestApplication/build_vs17_editor.sh](KotonoTestApplication/build_vs17_editor.sh)* for the solution with the editor.

    - *[KotonoTestApplication/build_vs17_game.sh](KotonoTestApplication/build_vs17_game.sh)* for the solution without the editor.

- All generated build files will be located in *KotonoTestApplication/build/*.

### Other IDE
- To build the solution for other IDEs, navigate to *[KotonoTestApplication/](KotonoTestApplication/)*, create a build folder and run CMake for your IDE with the option :

    - -DWITH_EDITOR=ON for the solution with the editor.

    - -DWITH_EDITOR=OFF for the solution without the editor.

## Shader Compilation
To enable automatic shader compilation, make sure you have the [Vulkan SDK](https://vulkan.lunarg.com/sdk/home) and that its bin folder is added to your system’s PATH environment variable.
