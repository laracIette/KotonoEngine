#include "ModelManager.h"
#include "Model.h"

void KtModelManager::Cleanup()
{
    for (auto& [path, model] : models_)
    {
        model->Cleanup();
        delete model;
    }
    models_.clear();
}

KtModel* KtModelManager::Get(const UPath& path)
{
    const auto it{ models_.find(path) };
    return it != models_.end() ? it->second : Create(path);
}

KtModel* KtModelManager::Create(const UPath& path)
{
    auto* model{ new KtModel(path) };
    model->Init();
    models_[path] = model;
    return model;
}