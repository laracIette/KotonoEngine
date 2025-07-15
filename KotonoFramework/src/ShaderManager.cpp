#include "ShaderManager.h"
#include "SpvCompiler.h"

void KtShaderManager::Init()
{
    KtSpvCompiler spvCompiler{};
    spvCompiler.CompileUpdated();
}

KtShader* KtShaderManager::Create(const std::filesystem::path& path)
{
    auto* shader = new KtShader(path);
    shader->Init();
    Add(path, shader);
    return shader;
}

void KtShaderManager::CleanupItem(KtShader* item) const
{
    item->Cleanup();
}
