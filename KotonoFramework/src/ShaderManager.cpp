#include "ShaderManager.h"

KtShader* KtShaderManager::Create(const std::filesystem::path& path)
{
    auto* item = new KtShader(path);
    item->Init();
    _items[path] = item;
    return item;
}

void KtShaderManager::CleanupItem(KtShader* item) const
{
    item->Cleanup();
}
