#include "SpvCompiler/SpvCompiler.h"

#include <File/File.h>
#include <format>
#include <Logging/log.h>
#include <nlohmann/json.hpp>
#include <Path/Path.h>
#include <Serializer/Serializer.h>

#define KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER ELogImportanceLevel::High

static const UPath ShadersPath{ "${ENGINE_DIRECTORY}/Shaders" };
#ifdef NDEBUG
static const UPath CompiledShadersPath{ "${ENGINE_DIRECTORY}/Cache/Shaders/Release/Compiled" };
static const UPath CompiledRegistryPath{ "${ENGINE_DIRECTORY}/Cache/Shaders/Release/compiled.ktregistry" };
static const UPath DependenciesRegistryPath{ "${ENGINE_DIRECTORY}/Cache/Shaders/Release/dependencies.ktregistry" };
#else
static const UPath CompiledShadersPath{ "${ENGINE_DIRECTORY}/Cache/Shaders/Debug/Compiled" };
static const UPath CompiledRegistryPath{ "${ENGINE_DIRECTORY}/Cache/Shaders/Debug/compiled.ktregistry" };
static const UPath DependenciesRegistryPath{ "${ENGINE_DIRECTORY}/Cache/Shaders/Debug/dependencies.ktregistry" };
#endif

static const std::array DependencyPaths{
    ShadersPath / "common.glsl",
    ShadersPath / "common_frag.glsl",
    ShadersPath / "common_vert.glsl",
    ShadersPath / "common_comp.glsl",
};

void SSpvCompiler::CompileAll()
{
    KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER, "Graphics", "Clearing registry...");

    SSerializer::Serialize(nlohmann::json::object(), CompiledRegistryPath);
    CompileUpdated();
}

void SSpvCompiler::CompileUpdated()
{
    // Create the compiled shaders directory if it doesn't exist
    std::filesystem::create_directories(CompiledShadersPath);

    if (HasDependenciesUpdated())
    {
        return CompileAll();
    }

    KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER, "Graphics", "compiling updated spirv shaders");

    nlohmann::json json{};
    SSerializer::Deserialize(json, CompiledRegistryPath);

    for (const auto* directory : { "vert", "frag", "comp" })
    {
        for (const auto& entry : std::filesystem::directory_iterator{ ShadersPath / directory })
        {
            // not a glsl file
            if (entry.path().extension() != std::format(".{0}", directory))
            {
                continue;
            }

            const auto entryPath{ std::format("{0}/{1}", directory, entry.path().filename().string()) };

            const auto time{ entry.last_write_time() };
            const auto formattedTime{ std::format("{0:%F}-{0:%T}", time) };

            b8 isInList{ false };
            for (auto& shader : json["shaders"])
            {
                if (shader["path"] != entryPath)
                {
                    continue;
                }

                if (shader["modified"] != formattedTime)
                {
                    if (Compile(entry.path()))
                    {
                        shader["modified"] = formattedTime;
                    }
                }

                isInList = true;
                break;
            }

            if (!isInList)
            {
                if (Compile(entry.path()))
                {
                    nlohmann::json shader{};
                    shader["path"] = entryPath;
                    shader["modified"] = formattedTime;
                    json["shaders"].push_back(shader);
                }
            }
        }
    }

    SSerializer::Serialize(json, CompiledRegistryPath);

    KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER, "Graphics", "compiled updated spirv shaders");
}

auto SSpvCompiler::HasDependenciesUpdated() -> b8
{
    nlohmann::json json{};
    SSerializer::Deserialize(json, DependenciesRegistryPath);
    
    b8 updated{ false };
    for (const auto& dependencyPath : DependencyPaths)
    {
        UFile const dependencyFile{ dependencyPath };
        auto const time{ dependencyFile.LastWriteTime() };
        auto const formattedTime{ std::format("{0:%F}-{0:%T}", time) };
        
        b8 isInList{ false };
        for (auto& dependency : json["dependencies"])
        {
            if (dependency["path"] != dependencyPath)
            {
                continue;
            }

            if (dependency["modified"] != formattedTime)
            {
                dependency["modified"] = formattedTime;
                updated = true;
            }

            isInList = true;
            break;
        }
        
        if (!isInList)
        {
            nlohmann::json dependency{};
            dependency["path"] = dependencyPath;
            dependency["modified"] = formattedTime;
            json["dependencies"].push_back(dependency);
            updated = true;
        }
    }

    SSerializer::Serialize(json, DependenciesRegistryPath);
    return updated;
}

auto SSpvCompiler::Compile(UPath const& path) -> b8
{
    UPath const filePath{ CompiledShadersPath / path.Name() + ".spv" };

    // User must have vulkan bin in environment variables path
    std::string command;
#ifdef NDEBUG
    command = std::format("glslc \"{0}\" -o \"{1}\"", path.ToPath().string(), filePath.ToPath().string());
#else
    command = std::format("glslc \"{0}\" -o \"{1}\" -g", path.ToPath().string(), filePath.ToPath().string());
#endif
    b8 const result{ std::system(command.c_str()) == 0 };

    if (result)
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER, "Graphics", "Successfully compiled shader {0}", filePath.ToPath().string());
    }
    else
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER, "Graphics", "Found errors while compiling shader {0}", filePath.ToPath().string());
    }

    return result;
}