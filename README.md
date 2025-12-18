# Kotono Engine

## Build

These instructions are to be run within your project folder, see the *[Kotono Template Project](https://github.com/laracIette/KotonoTemplateProject)*.

- To build the solution run *[build.sh](build.sh)*.

    - All generated build files will be located in *build/*.

- Or create a build folder and run CMake for your IDE with the options :

    - -DWITH_EDITOR=*ON* for the solution with the editor.

    - -DWITH_EDITOR=*OFF* for the solution without the editor.

    - -DENGINE_PATH:PATH=*Path/To/Your/KotonoEngine* so that the project finds the engine.

## Side Note
Make sure that you have the *[Vulkan SDK](https://vulkan.lunarg.com/sdk/home)* and that its bin folder is added to your system’s PATH environment variable.
