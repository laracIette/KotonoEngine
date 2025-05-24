#include "ShaderManager.h"

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
