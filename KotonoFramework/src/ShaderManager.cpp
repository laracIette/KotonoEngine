#include "ShaderManager.h"

KtShader* KtShaderManager::Create(const std::filesystem::path& path)
{
    if (auto* shader = Get(path))
    {
        return shader;
    }

    auto* shader = new KtShader(path);
    shader->Init();
    items_[path] = shader;
    return shader;
}

void KtShaderManager::CleanupItem(KtShader* item) const
{
    item->Cleanup();
}
