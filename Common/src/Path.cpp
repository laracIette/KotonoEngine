#include "Path.h"

std::filesystem::path KtPath::Graphics()
{
    return std::filesystem::path(ENGINE_DIRECTORY) / "Graphics";
}

const std::filesystem::path& KtPath::Project()
{
    return projectPath_;
}
