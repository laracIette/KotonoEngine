#include "KtSpvCompiler.h"
#include "Framework.h"
#include "Path.h"
#include "log.h"

void KtSpvCompiler::CompileAll() const
{
    KT_DEBUG_LOG("compiling all spirv shaders");

    const auto path = Framework.GetPath().GetFrameworkPath() / "shaders";

    for (const auto& entry : std::filesystem::directory_iterator(path / "vert"))
    {
        if (entry.path().extension() == ".vert")
        {
            Compile(entry.path());
        }
    }
    for (const auto& entry : std::filesystem::directory_iterator(path / "frag"))
    {
        if (entry.path().extension() == ".frag")
        {
            Compile(entry.path());
        }
    }

    KT_DEBUG_LOG("compiled all spirv shaders");
}

void KtSpvCompiler::Compile(const std::filesystem::path& path) const
{
    KT_DEBUG_LOG("%s", path.string().c_str());
    const auto command = std::format("{} \"{}\" -o \"{}\".spv", R"(C:\VulkanSDK\1.3.236.0\Bin\glslc.exe)", path.string(), path.string());
    std::system(command.c_str());
}

//void KtSpvCompiler::WriteSPIRVToFile(const std::span<uint32_t> spirv, const std::filesystem::path& path) const
//{
//    const auto file = KtFile(path);
//    file.WriteBinary(spirv);
//}