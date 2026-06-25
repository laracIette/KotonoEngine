#include "SpvCompiler.h"
#include <format>
#include <kotono_common/log.h>
#include <kotono_common/Path.h>
#include <kotono_io/serialize_base.h>
#include <kotono_io/Serializer.h>
#include <nlohmann/json.hpp>

#define KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER ELogImportanceLevel::High

void KtSpvCompiler::CompileUpdated() const
{
    KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER, "Graphics", "compiling updated spirv shaders");

    static const UPath shadersPath{ "${ENGINE_DIRECTORY}/Graphics/shaders" };
    static const UPath registryPath{ shadersPath / "shaders.ktregistry" };

    nlohmann::json json{};
    USerializer::Deserialize(json, registryPath);

    for (const auto* directory : { "vert", "frag" })
    {
        for (const auto& entry : std::filesystem::directory_iterator{ shadersPath / directory })
        {
            // not a glsl file
            if (entry.path().extension() != std::format(".{0}", directory))
            {
                continue;
            }

            const auto entryPath{ std::format("{0}/{1}", directory, entry.path().filename().string()) };

            const auto ftime{ entry.last_write_time() };
            const auto formattedTime{ std::format("{0:%F}-{0:%T}", ftime) };

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

    USerializer::Serialize(json, registryPath);

    KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER, "Graphics", "compiled updated spirv shaders");
}

void KtSpvCompiler::CompileAll() const
{
    KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER, "Graphics", "compiling all spirv shaders");

    static const UPath shadersPath{ "${ENGINE_DIRECTORY}/Graphics/shaders" };
    static const UPath registryPath{ shadersPath / "shaders.ktregistry" };

    nlohmann::json json{};
    make_array(json["shaders"]);

    for (const auto* directory : { "vert", "frag" })
    {
        for (const auto& entry : std::filesystem::directory_iterator{ shadersPath / directory })
        {
            if (entry.path().extension() != std::format(".{0}", directory))
            {
                continue;
            }

            const auto entryPath{ std::format("{0}/{1}", directory, entry.path().filename().string()) };
            
            const auto ftime{ entry.last_write_time() };
            const auto formattedTime{ std::format("{0:%F}-{0:%T}", ftime) };

            if (Compile(entry.path()))
            {
                nlohmann::json shader{};
                shader["path"] = entryPath;
                shader["modified"] = formattedTime;
                json["shaders"].push_back(shader);
            }
        }
    }

    USerializer::Serialize(json, registryPath);

    KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER, "Graphics", "compiled all spirv shaders");
}

bool KtSpvCompiler::Compile(const std::filesystem::path& path) const
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