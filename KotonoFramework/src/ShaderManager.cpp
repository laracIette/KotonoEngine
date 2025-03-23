#include "ShaderManager.h"

KtShader* KtShaderManager::CreateItem(const std::filesystem::path& path) const
{
    return nullptr;
}

void KtShaderManager::CleanupItem(KtShader* item) const
{
    item->Cleanup();
}
