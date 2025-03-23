#include "ImageTextureManager.h"

KtImageTexture* KtImageTextureManager::CreateItem(const std::filesystem::path& path) const
{
    auto* item = new KtImageTexture(path);
    item->Init();
    return item;
}

void KtImageTextureManager::CleanupItem(KtImageTexture* item) const
{
    item->Cleanup();
}
