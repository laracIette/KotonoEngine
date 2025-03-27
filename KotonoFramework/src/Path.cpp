#include "Path.h"
#include <Windows.h>

void KtPath::Init()
{
    char path[MAX_PATH];
    GetModuleFileNameA(nullptr, path, MAX_PATH);

    _solutionPath = std::filesystem::path(path).parent_path().parent_path().parent_path();
    _frameworkPath = _solutionPath / R"(KotonoFramework)";
    _enginePath = _solutionPath / R"(KotonoEngine)";
}

void KtPath::Cleanup()
{
}

const std::filesystem::path& KtPath::GetSolutionPath() const
{
    return _solutionPath;
}

const std::filesystem::path& KtPath::GetFrameworkPath() const
{
    return _frameworkPath;
}

const std::filesystem::path& KtPath::GetEnginePath() const
{
    return _enginePath;
}