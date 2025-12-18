# Kotono Engine

## Build

These instructions are to be run within your project folder, see the *[KotonoTemplateProject](https://github.com/laracIette/KotonoTemplateProject)*.

### Visual Studio
- To build the solution for Visual Studio, execute *[build_vs17.sh](build_vs17.sh)* for Visual Studio 2022, or *[build_vs18.sh](build_vs18.sh)* for Visual Studio 2026.

- All generated build files will be located in *build/*.

### Other IDE
- To build the solution for other IDEs, create a build folder and run CMake for your IDE with the options :

    - -DWITH_EDITOR=*ON* for the solution with the editor.

    - -DWITH_EDITOR=*OFF* for the solution without the editor.

    - -DENGINE_PATH:PATH=*Path/To/Your/KotonoEngine* so that the project finds the engine.

## Side Note
Make sure that you have the *[Vulkan SDK](https://vulkan.lunarg.com/sdk/home)* and that its bin folder is added to your system’s PATH environment variable.
