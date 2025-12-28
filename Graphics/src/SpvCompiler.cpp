#include "SpvCompiler.h"
#include <format>
#include <kotono_common/log.h>
#include <kotono_common/Path.h>
#include <kotono_io/Serializer.h>
#include <nlohmann/json.hpp>

#define KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER ELogImportanceLevel::High

void KtSpvCompiler::CompileUpdated() const
{
    KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER, "Graphics.KtSpvCompiler::CompileAll()", "compiling updated spirv shaders");

    static const auto path{ KtPath::Graphics() / "shaders" };
    static const auto registryPath{ path / "shaders.ktregistry" };

    nlohmann::json json{};
    KtSerializer::Deserialize(json, registryPath);

    for (const auto& directory : { "vert", "frag" })
    {
        for (const auto& entry : std::filesystem::directory_iterator(path / directory))
        {
            // not a glsl file
            if (entry.path().extension() != std::format(".{}", directory))
            {
                continue;
            }

            const auto entryPath = std::format("{}/{}", directory, entry.path().filename().string());

            const auto ftime = entry.last_write_time();
            const auto formattedTime = std::format("{0:%F}-{0:%T}", ftime);

            bool isInList = false;
            for (auto& shader : json["shaders"])
            {
                if (shader["path"] != entryPath)
                {
                    continue;
                }

                if (shader["modified"] != formattedTime)
                {
                    shader["modified"] = formattedTime;
                    Compile(entry.path());
                }

                isInList = true;
                break;
            }

            if (!isInList)
            {
                nlohmann::json shader{};
                shader["path"] = entryPath;
                shader["modified"] = formattedTime;
                json["shaders"].push_back(shader);
                Compile(entry.path());
            }
        }
    }

    KtSerializer::Serialize(json, registryPath);

    KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER, "Graphics.KtSpvCompiler::CompileAll()", "compiled updated spirv shaders");
}

void KtSpvCompiler::CompileAll() const
{
    KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER, "Graphics.KtSpvCompiler::CompileAll()", "compiling all spirv shaders");

    const auto path = KtPath::Graphics() / "shaders";
    for (const auto& directory : { "vert", "frag" })
    {
        for (const auto& entry : std::filesystem::directory_iterator(path / directory))
        {
            if (entry.path().extension() == std::format(".{}", directory))
            {
                Compile(entry.path());
            }
        }
    }

    KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER, "Graphics.KtSpvCompiler::CompileAll()", "compiled all spirv shaders");
}

void KtSpvCompiler::Compile(const std::filesystem::path& path) const
{
    KT_LOG(KT_LOG_IMPORTANCE_LEVEL_SPV_COMPILER, "Graphics.KtSpvCompiler::Compile()", "compiling shader %s", path.string().c_str());
    // must have vulkan bin in environment variables path
    const auto command{ std::format("glslc \"{}\" -o \"{}\".spv", path.string(), path.string()) };
    std::system(command.c_str());
}