#include "ImageTextureManager.h"
#include "ImageTexture.h"

void KtImageTextureManager::Cleanup()
{
    for (auto& [path, imageTexture] : imageTextures_)
    {
        imageTexture->Cleanup();
        delete imageTexture;
    }
    imageTextures_.clear();
}

KtImageTexture* KtImageTextureManager::Get(const std::filesystem::path& path)
{
    const auto it = imageTextures_.find(path);
    if (it != imageTextures_.end())
    {
        return it->second;
    }
    return Create(path);
}

KtImageTexture* KtImageTextureManager::Create(const std::filesystem::path& path)
{
    auto* imageTexture = new KtImageTexture(path);
    imageTexture->Init();
    imageTextures_[path] = imageTexture;
    return imageTexture;
}
