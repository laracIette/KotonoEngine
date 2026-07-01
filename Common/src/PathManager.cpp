#include "PathManager.h"

const std::filesystem::path& SPathManager::Engine()
{
    static std::filesystem::path engineDirectory{ ENGINE_DIRECTORY };
    return engineDirectory;
}

const std::filesystem::path& SPathManager::Project()
{
    return projectPath_;
}
