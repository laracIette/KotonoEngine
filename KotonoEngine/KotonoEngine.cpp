#include "KotonoEngine.h"

KotonoEngine::KotonoEngine() :
    _projectDirectory(R"(C:\Users\nicos\Documents\Kotono Projects\Test)")
{
}

const std::filesystem::path& KotonoEngine::GetProjectDirectory() const
{
    return _projectDirectory;
}