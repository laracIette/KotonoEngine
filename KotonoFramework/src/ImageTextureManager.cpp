#include "ImageTextureManager.h"

KtImageTexture* KtImageTextureManager::Create(const std::filesystem::path& path)
{
    auto* item = new KtImageTexture(path);
    item->Init();
    _items[path] = item;
    return item;
}

void KtImageTextureManager::CleanupItem(KtImageTexture* item) const
{
    item->Cleanup();
}
