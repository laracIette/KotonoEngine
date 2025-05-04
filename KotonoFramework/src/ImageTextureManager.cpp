#include "ImageTextureManager.h"

KtImageTexture* KtImageTextureManager::Create(const std::filesystem::path& path)
{
    if (auto* imageTexture = Get(path))
    {
        return imageTexture;
    }

    auto* imageTexture = new KtImageTexture(path);
    imageTexture->Init();
    _items[path] = imageTexture;
    return imageTexture;
}

void KtImageTextureManager::CleanupItem(KtImageTexture* item) const
{
    item->Cleanup();
}
