#include "ModelManager.h"
#include "Model.h"

KtModel* KtModelManager::Create(const std::filesystem::path& path)
{
    if (auto* model = Get(path))
    {
        return model;
    }

    auto* model = new KtModel(path);
    model->Init();
    items_[path] = model;
    return model;
}

void KtModelManager::CleanupItem(KtModel* item) const
{
    item->Cleanup();
}