#include "ModelWireframeManager.h"

KtModelWireframe* KtModelWireframeManager::Create(const std::filesystem::path& path)
{
    if (auto* modelWireframe = Get(path))
    {
        return modelWireframe;
    }

    auto* modelWireframe = new KtModelWireframe(path);
    modelWireframe->Init();
    _items[path] = modelWireframe;
    return modelWireframe;
}

void KtModelWireframeManager::CleanupItem(KtModelWireframe* item) const
{
    item->Cleanup();
}