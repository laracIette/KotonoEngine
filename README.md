# Kotono Engine

The Kotono Engine is a 3D game engine built using C++ and the Vulkan API.

## Summary

- [1. Building](#1-building)
    - [Requirements](#requirements)
    - [Instructions](#instructions)
- [2. Structure overview](#2-structure-overview)
    - [Framework](#framework)
    - [Core](#core)
    - [Editor](#editor)
- [3. What's next ?](#3-whats-next-)
    - [Draw indexed](#draw-indexed)
    - [Reflection](#reflection)

## 1. Building

### Requirements

- *[CMake](https://cmake.org/download/)* 4.2 or later.

- The *[Vulkan SDK](https://vulkan.lunarg.com/sdk/home)* 1.4 or later, with its bin folder added to the system’s PATH environment variable.

- A C++ 23 compiler. *[MSVC](https://visualstudio.microsoft.com/downloads/)* is guaranteed to work, but others should work too.

### Instructions

The build takes place from a project, see the *[Kotono Template Project](https://github.com/laracIette/KotonoTemplateProject)*.

## 2. Structure overview

The Kotono Engine is structured with a modular architecture in mind. It is divided into multiple projects and each project only includes the necessary dependencies to optimize build times.

### Framework

Kotono is built from the ground up using only the Vulkan API for graphics. It features a multi-stage rendering pipeline with deferred lighting, view the rendering pipeline [here](https://github.com/laracIette/KotonoEngine/blob/952dd7c2f57ccfcc23243680cd73a35cc99f02b3/Graphics/src/Renderer.cpp#L525).

Kotono features an asset system which helps simplify the workflow for creating resources such as [shaders](Graphics/assets/shaders/gbuffer.kasset). It uses *[nlohmann's json](https://github.com/nlohmann/json)* library combined with the [Serializer](IO/include/kotono_io/Serializer.h) class for serializing and deserializing assets.

### Core

In Kotono, all instantiable objects inherit from a base [Object](Object/include/kotono_object/Object.h) class, this class provides a bunch of utilities such as debug information (source file, line, ...), automatic serialization and deserialization using the *GENERATED_T()* macro combined with the *SERIALIZE* macro on serializable fields.

Every object must be instantiated using the [Create](https://github.com/laracIette/KotonoEngine/blob/952dd7c2f57ccfcc23243680cd73a35cc99f02b3/Object/include/kotono_object/Object.h#L125) struct, this struct provides a pointer to the instantiated object that automatically gets invalidated when the object gets destroyed.

The object header also provides macros such as [ReadonlyProperty](https://github.com/laracIette/KotonoEngine/blob/952dd7c2f57ccfcc23243680cd73a35cc99f02b3/Object/include/kotono_object/Object.h#L33) or [WritableProperty](https://github.com/laracIette/KotonoEngine/blob/952dd7c2f57ccfcc23243680cd73a35cc99f02b3/Object/include/kotono_object/Object.h#L39) for convenience.

Spawnable objects are built using an instance of the [Scene Object](Core/include/kotono_core/SceneObject.h) class to which instances of [Scene Components](Core/include/kotono_core/SceneComponent.h) are added. The scene object acts as a handle and parent for scene components that contain the actual gameplay logic.

### Editor

The editor layer is divided into multiple projects, those are included in the application project only if it was built with the CMake option *WITH_EDITOR*.

Interfaces are created with a widget system where every widget class inherits from a base [Widget](Interface/include/kotono_interface/Widget.h) class that itself inherits from the [Object](Object/include/kotono_object/Object.h) class. It is inspired by *[Flutter](https://flutter.dev/)* and an example of a widget build can be found in the [Main Window](https://github.com/laracIette/KotonoEngine/blob/952dd7c2f57ccfcc23243680cd73a35cc99f02b3/Editor/src/MainWindow.cpp#L27) widget class.

Widgets are updated by calling the [SetState](https://github.com/laracIette/KotonoEngine/blob/952dd7c2f57ccfcc23243680cd73a35cc99f02b3/Interface/include/kotono_interface/Widget.h#L77) function, which refreshes their displayed content. The widget header also provides the [StateProperty](https://github.com/laracIette/KotonoEngine/blob/952dd7c2f57ccfcc23243680cd73a35cc99f02b3/Interface/include/kotono_interface/Widget.h#L28) macro that automatically refreshes the widget after updating the property's value.

The interface doesn't depend on any GUI library and is entirely rendered using Kotono's [renderer](Graphics/include/kotono_graphics/Renderer.h).

## 3. What's next ?

A bunch of issues are planned on the *[Kotono project board](https://github.com/users/laracIette/projects/7/views/1)*. Below are some of the most important ones.

### Draw indexed

Kotono is currently submitting one draw command per renderable object. I am trying to figure out how do use *[indexed drawing with draw command buffers](https://docs.vulkan.org/refpages/latest/refpages/source/vkCmdDrawIndexedIndirectCount.html)* while keeping multiple graphics pipelines. This will drastically reduce the number of draw calls from one per renderable to about one per pipeline.

### Reflection

The Kotono Engine is currently using its own reflection project that generates header and source files for each KObject class. In the future, perhaps *[C++26's compile-time reflection](https://isocpp.org/files/papers/P2996R13.html)* will replace or at least help build a less complex and more performant reflection system.