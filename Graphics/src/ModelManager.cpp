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

KtModel* KtModelManager::Get(const std::filesystem::path& path)
{
    const auto it = models_.find(path);
    if (it != models_.end())
    {
        return it->second;
    }
    return Create(path);
}

KtModel* KtModelManager::Create(const std::filesystem::path& path)
{
    auto* model = new KtModel(path);
    model->Init();
    models_[path] = model;
    return model;
}