#include "SpvCompiler.h"
#include "Framework.h"
#include "Path.h"
#include "log.h"
#include <nlohmann/json.hpp>
#include "Serializer.h"
#include <format>

void KtSpvCompiler::CompileUpdated() const
{
    const auto path = Framework.GetPath().GetFrameworkPath() / "shaders";
    nlohmann::json json;
    KtSerializer serializer;

    serializer.ReadData(path / "shaders.ktregistry", json);
    for (const auto& directory : { "vert", "frag" })
    {
        for (const auto& entry : std::filesystem::directory_iterator(path / directory))
        {
            // not a glsl file
            if (entry.path().extension() != std::format(".{}", directory))
            {
                continue;
            }

            const auto entryPath = std::format("{}/{}{}", directory, entry.path().filename().string(), entry.path().extension().string());

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
                nlohmann::json shader;
                shader["path"] = entryPath;
                shader["modified"] = formattedTime;
                json["shaders"].push_back(shader);
                Compile(entry.path());
            }
        }
    }

    serializer.WriteData(path / "shaders.ktregistry", json);
}

void KtSpvCompiler::CompileAll() const
{
    KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_HIGH, "compiling all spirv shaders");

    const auto path = Framework.GetPath().GetFrameworkPath() / "shaders";
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

    KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_HIGH, "compiled all spirv shaders");
}

void KtSpvCompiler::Compile(const std::filesystem::path& path) const
{
    KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_HIGH, "%s", path.string().c_str());
    // must have vulkan bin in environment variables path
    const auto command = std::format("glslc \"{}\" -o \"{}\".spv", path.string(), path.string());
    std::system(command.c_str());
}