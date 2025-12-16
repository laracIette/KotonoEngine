#include "Path.h"

void KtPath::Init()
{
    rootPath_ = ENGINE_DIRECTORY;
    frameworkPath_ = rootPath_ / "Graphics";
    enginePath_ = rootPath_ / "Core";
}

const std::filesystem::path& KtPath::Root() const
{
    return rootPath_;
}

const std::filesystem::path& KtPath::Framework() const
{
    return frameworkPath_;
}

const std::filesystem::path& KtPath::Engine() const
{
    return enginePath_;
}

const std::filesystem::path& KtPath::Project() const
{
    return projectPath_;
}
