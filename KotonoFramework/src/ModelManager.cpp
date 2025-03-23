#include "ModelManager.h"

KtModel* KtModelManager::CreateItem(const std::filesystem::path& path) const
{
    auto* item = new KtModel(path);
    item->Init();
    return item;
}

void KtModelManager::CleanupItem(KtModel* item) const
{
    item->Cleanup();
}