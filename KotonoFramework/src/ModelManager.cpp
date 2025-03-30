#include "ModelManager.h"

KtModel* KtModelManager::Create(const std::filesystem::path& path)
{
    auto* item = new KtModel(path);
    item->Init();
    _items[path] = item;
    return item;
}

void KtModelManager::CleanupItem(KtModel* item) const
{
    item->Cleanup();
}