#include "PathManager.h"

UPath const& SPathManager::Engine()
{
    static UPath engineDirectory{ ENGINE_DIRECTORY };
    return engineDirectory;
}

UPath const& SPathManager::Project()
{
    return projectPath_;
}
