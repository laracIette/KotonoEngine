#include "PathManager.h"

std::filesystem::path SPathManager::Engine() const
{
    return ENGINE_DIRECTORY;
}

const std::filesystem::path& SPathManager::Project() const
{
    return projectPath_;
}
