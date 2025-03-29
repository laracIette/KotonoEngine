#include "ShaderManager.h"

KtShader* KtShaderManager::CreateItem(const std::filesystem::path& path) const
{
    auto* item = new KtShader(path);
    item->Init();
    return item;
}

void KtShaderManager::CleanupItem(KtShader* item) const
{
    item->Cleanup();
}
