#include "Path.h"
#include <Windows.h>

void KtPath::Init()
{
    char path[MAX_PATH];
    GetModuleFileNameA(nullptr, path, MAX_PATH);

    rootPath_ = std::filesystem::path(path).parent_path().parent_path().parent_path().parent_path();
    frameworkPath_ = rootPath_ / R"(KotonoFramework)";
    enginePath_ = rootPath_ / R"(KotonoEngine)";
    projectPath_ = rootPath_ / R"(KotonoTestApplication)";
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
