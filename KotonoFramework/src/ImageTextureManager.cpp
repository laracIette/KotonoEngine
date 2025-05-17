#include "ImageTextureManager.h"
#include "ImageTexture.h"

KtImageTexture* KtImageTextureManager::Create(const std::filesystem::path& path)
{
    if (auto* imageTexture = Get(path))
    {
        return imageTexture;
    }

    auto* imageTexture = new KtImageTexture(path);
    imageTexture->Init();
    items_[path] = imageTexture;
    return imageTexture;
}

void KtImageTextureManager::CleanupItem(KtImageTexture* item) const
{
    item->Cleanup();
}
