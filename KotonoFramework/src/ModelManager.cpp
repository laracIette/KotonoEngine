#include "ModelManager.h"
#include "Model.h"

KtModel* KtModelManager::Create(const std::filesystem::path& path)
{    
    auto* model = new KtModel(path);
    model->Init();
    Add(path, model);
    return model;
}

void KtModelManager::CleanupItem(KtModel* item) const
{
    item->Cleanup();
}