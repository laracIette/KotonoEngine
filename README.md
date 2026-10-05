# Kotono Engine

The Kotono Engine is a 3D game engine built using C++ and the Vulkan API.

## Summary

- [1. Building](#1-building)
    - [Requirements](#requirements)
    - [Instructions](#instructions)
- [2. Structure overview](#2-structure-overview)
    - [Rendering](#rendering)
    - [Object system](#object-system)
    - [Editor](#editor)
- [3. What's next ?](#3-whats-next-)
    - [Reflection](#reflection)

## 1. Building

### Requirements

- *[CMake](https://cmake.org/download/)* 4.2 or later.

- The *[Vulkan SDK](https://vulkan.lunarg.com/sdk/home)* 1.4 or later, with its bin folder added to the system’s PATH environment variable.

- A C++ 23 compiler. As of today, only *[MSVC](https://visualstudio.microsoft.com/downloads/)* is guaranteed to work.

### Instructions

The build takes place from a project, see the *[Kotono Template Project](https://github.com/laracIette/KotonoTemplateProject#readme)*.

## 2. Structure overview

The Kotono Engine is structured with a modular architecture in mind. It is divided into multiple projects and each project only includes necessary dependencies to optimize build times.

### Rendering

Kotono is built from the ground up using the Vulkan API for graphics. It features a multi-stage deferred rendering pipeline with basic directional light shadows.

Kotono features an asset system which helps simplify the workflow for creating resources such as [shaders](https://github.com/laracIette/KotonoEngine/blob/42603a3fbc84a5e61ef86cc8e58d1fd273c4351c/Assets/shaders/gbuffer.kasset). It uses *[nlohmann's json](https://github.com/nlohmann/json)* library combined with the [Serializer](https://github.com/laracIette/KotonoEngine/blob/42603a3fbc84a5e61ef86cc8e58d1fd273c4351c/Source/Core/IO/Public/Serializer/Serializer.h) class which provides utilities for serializing and deserializing json files.

Kotono supports multi-windowing, secondary windows can currently be created by detaching a [Detachable Widget](https://github.com/laracIette/KotonoEngine/blob/42603a3fbc84a5e61ef86cc8e58d1fd273c4351c/Source/Editor/Private/Detachable/Detachable.h) from the interface.

### Object system

In Kotono, all instantiable objects inherit from a base [Object](https://github.com/laracIette/KotonoEngine/blob/42603a3fbc84a5e61ef86cc8e58d1fd273c4351c/Source/Core/Object/Public/Object/Object.h) class, this class provides a bunch of utilities such as debug information (source file, line, ...), automatic serialization and deserialization using the *GENERATED()* reflection macro combined with the *SERIALIZE* macro on serializable fields.

Every object must be instantiated using the *UCreate* struct, this struct provides a pointer to the instantiated object that automatically gets invalidated when the object gets destroyed, view the smart pointer [here](https://github.com/laracIette/KotonoEngine/blob/42603a3fbc84a5e61ef86cc8e58d1fd273c4351c/Source/Core/Object/Public/Ptr.h).

The object header also provides macros such as *ReadonlyProperty* or *WritableProperty* for convenience. These macros generate getters and setters given a set of parameters.

Spawnable objects are built using the [Scene Object](https://github.com/laracIette/KotonoEngine/blob/42603a3fbc84a5e61ef86cc8e58d1fd273c4351c/Source/Core/Object/Public/SceneObject/SceneObject.h) class which owns various [Scene Components](https://github.com/laracIette/KotonoEngine/blob/42603a3fbc84a5e61ef86cc8e58d1fd273c4351c/Source/Core/Object/Public/SceneComponent/SceneComponent.h). The scene object acts as a handle and parent for scene components that contain the actual gameplay logic.

### Editor

The editor layer is included in the application project only if the CMake option *WITH_EDITOR* was set.

Interfaces are created with a widget system. Every widget inherits from a base [Widget](https://github.com/laracIette/KotonoEngine/blob/42603a3fbc84a5e61ef86cc8e58d1fd273c4351c/Source/Core/Object/Public/Widget/Widget.h) class. An example of a widget build can be found in the [Default Scene Context](https://github.com/laracIette/KotonoEngine/blob/42603a3fbc84a5e61ef86cc8e58d1fd273c4351c/Source/Editor/Private/DefaultSceneContext/DefaultSceneContext.cpp) widget's build function. The [Widget](https://github.com/laracIette/KotonoEngine/blob/42603a3fbc84a5e61ef86cc8e58d1fd273c4351c/Source/Core/Object/Public/Widget/Widget.h) class itself inherits from the [Object](https://github.com/laracIette/KotonoEngine/blob/42603a3fbc84a5e61ef86cc8e58d1fd273c4351c/Source/Core/Object/Public/Object/Object.h) class, which provides reflection and serialization for all widget classes.

Widgets are updated by calling the *SetState* function, which refreshes their displayed content. The widget header also provides the *StateProperty* macro that automatically refreshes the widget after updating the property's value. Widgets can also override the *GetCanCache()* function to specify whether a certain widget caches or refreshes each frame.

The interface doesn't depend on any GUI library and is entirely rendered using Kotono's [Renderer](https://github.com/laracIette/KotonoEngine/blob/42603a3fbc84a5e61ef86cc8e58d1fd273c4351c/Source/Core/Rendering/Public/Renderer/Renderer.h).

## 3. What's next ?

A bunch of issues are planned on the *[Kotono project board](https://github.com/users/laracIette/projects/7/views/1)*. Below are some of the most important ones.

### Reflection

The Kotono Engine is currently using its own reflection project that generates header and source files for each [Object](https://github.com/laracIette/KotonoEngine/blob/42603a3fbc84a5e61ef86cc8e58d1fd273c4351c/Source/Core/Object/Public/Object/Object.h) class. In the future, *[C++26's compile-time reflection](https://isocpp.org/files/papers/P2996R13.html)* will help build a more performant and safer reflection system.