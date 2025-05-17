#include "Path.h"
#include <Windows.h>

void KtPath::Init()
{
    char path[MAX_PATH];
    GetModuleFileNameA(nullptr, path, MAX_PATH);

    solutionPath_ = std::filesystem::path(path).parent_path().parent_path().parent_path();
    frameworkPath_ = solutionPath_ / R"(KotonoFramework)";
    enginePath_ = solutionPath_ / R"(KotonoEngine)";
}

void KtPath::Cleanup()
{
}

const std::filesystem::path& KtPath::GetSolutionPath() const
{
    return solutionPath_;
}

const std::filesystem::path& KtPath::GetFrameworkPath() const
{
    return frameworkPath_;
}

const std::filesystem::path& KtPath::GetEnginePath() const
{
    return enginePath_;
}