#include "SpvCompiler.h"
#include <format>
#include <kotono_common/log.h>
#include <kotono_common/Path.h>
#include <kotono_io/File.h>
#include <kotono_io/Serializer.h>
#include <nlohmann/json.hpp>

#define KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER ELogImportanceLevel::High

static const UPath ShadersPath{ "${ENGINE_DIRECTORY}/Graphics/shaders" };
static const UPath ShaderRegistryPath{ ShadersPath / "shaders.ktregistry" };

static const UPath DependencyRegistryPath{ ShadersPath / "dependencies.ktregistry" };
static const std::array DependencyPaths{
    "common.glsl",
};

void USpvCompiler::CompileAll()
{
    KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER, "Graphics", "Clearing registry...");

    USerializer::Serialize(nlohmann::json::object(), ShaderRegistryPath);
    CompileUpdated();
}

void USpvCompiler::CompileUpdated()
{
    if (DependenciesUpdated())
    {
        return CompileAll();
    }

    KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER, "Graphics", "compiling updated spirv shaders");

    nlohmann::json json{};
    USerializer::Deserialize(json, ShaderRegistryPath);

    for (const auto* directory : { "vert", "frag" })
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

            bool isInList{ false };
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

    USerializer::Serialize(json, ShaderRegistryPath);

    KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER, "Graphics", "compiled updated spirv shaders");
}

bool USpvCompiler::DependenciesUpdated()
{
    nlohmann::json json{};
    USerializer::Deserialize(json, DependencyRegistryPath);
    
    bool updated{ false };
    for (const auto& dependencyPath : DependencyPaths)
    {
        const UFile dependencyFile{ ShadersPath / dependencyPath };
        const auto time{ dependencyFile.LastWriteTime() };
        const auto formattedTime{ std::format("{0:%F}-{0:%T}", time) };
        
        bool isInList{ false };
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

    USerializer::Serialize(json, DependencyRegistryPath);
    return updated;
}

bool USpvCompiler::Compile(const std::filesystem::path& path)
{
    // user must have vulkan bin in environment variables path
    const auto command{ std::format("glslc \"{0}\" -o \"{0}\".spv", path.string()) };
    const bool result{ std::system(command.c_str()) == 0 };

    if (result)
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER, "Graphics", "Successfully compiled shader {0}", path.string());
    }
    else
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER, "Graphics", "Found errors while compiling shader {0}", path.string());
    }

    return result;
}