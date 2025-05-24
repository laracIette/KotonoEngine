#include "ImageTextureManager.h"
#include "ImageTexture.h"

KtImageTexture* KtImageTextureManager::Create(const std::filesystem::path& path)
{
    auto* imageTexture = new KtImageTexture(path);
    imageTexture->Init();
    Add(path, imageTexture);
    return imageTexture;
}

void KtImageTextureManager::CleanupItem(KtImageTexture* item) const
{
    item->Cleanup();
}
