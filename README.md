# Kotono Engine

## Build

These instructions are to be run within your project folder, see the *[Kotono Template Project](https://github.com/laracIette/KotonoTemplateProject)*.

- Run *[generate_cmakepresets.sh](generate_cmakepresets.sh)* to generate *CMakePresets.json*, this allows your IDE or the command line to recognize the project's build configurations.

    - Build via your IDE if it supports cmake presets.

    - Else, use the command 'cmake --preset default' then 'cmake --build --preset release' to build manually.

        - Replace 'release' with 'debug' for debugging symbols.

        - All generated build files will be located in *build/*.

- Or create a build folder and run CMake for your IDE with the options :

    - -DWITH_EDITOR=*ON* for the solution with the editor.

    - -DWITH_EDITOR=*OFF* for the solution without the editor.

    - -DENGINE_PATH:PATH=*Path/To/Your/KotonoEngine* so that the project finds the engine.

## Side Note
Make sure that you have the *[Vulkan SDK](https://vulkan.lunarg.com/sdk/home)* and that its bin folder is added to your system’s PATH environment variable.
