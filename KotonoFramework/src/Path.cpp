#include "Path.h"
#include <Windows.h>

void KtPath::Init()
{
    char path[MAX_PATH];
    GetModuleFileNameA(nullptr, path, MAX_PATH);

    rootPath_ = std::filesystem::path(path).parent_path().parent_path().parent_path().parent_path();
    frameworkPath_ = rootPath_ / R"(KotonoFramework)";
    enginePath_ = rootPath_ / R"(KotonoEngine)";
}

const std::filesystem::path& KtPath::RootPath() const
{
    return rootPath_;
}

const std::filesystem::path& KtPath::FrameworkPath() const
{
    return frameworkPath_;
}

const std::filesystem::path& KtPath::EnginePath() const
{
    return enginePath_;
}