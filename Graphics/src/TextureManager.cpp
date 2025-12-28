#include "TextureManager.h"
#include "Texture.h"

void KtTextureManager::Cleanup()
{
    for (auto& [path, texture] : textures_)
    {
        texture->Cleanup();
        delete texture;
    }
    textures_.clear();
}

KtTexture* KtTextureManager::Get(const std::filesystem::path& path)
{
    const auto it{ textures_.find(path) };
    return it != textures_.end() ? it->second : Create(path);
}

KtTexture* KtTextureManager::Create(const std::filesystem::path& path)
{
    auto* texture{ new KtTexture(path) };
    texture->Init();
    textures_[path] = texture;
    return texture;
}
