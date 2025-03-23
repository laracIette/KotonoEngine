#include "ImageTextureManager.h"
#include "log.h"

KtImageTexture* KtImageTextureManager::Get(const std::filesystem::path& path)
{
    if (!_imageTextures.contains(path))
    {
        auto* imageTexture = new KtImageTexture(path);
        imageTexture->Init();

        _imageTextures[path] = imageTexture;
    }

    return _imageTextures[path];
}

void KtImageTextureManager::Init()
{
}

void KtImageTextureManager::Cleanup()
{
    KT_DEBUG_LOG("cleaning up image texture manager");

    for (auto& [path, imageTexture] : _imageTextures)
    {
        imageTexture->Cleanup();
        delete imageTexture;
    }

    KT_DEBUG_LOG("cleaned up image texture manager");
}
