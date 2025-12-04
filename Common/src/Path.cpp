#include "Path.h"
//#include "Serializer.h"
#include <nlohmann/json.hpp>
#include <Windows.h>

void KtPath::Init()
{
    char executablePath[MAX_PATH];
    GetModuleFileNameA(nullptr, executablePath, MAX_PATH);

    const auto executableDirectory{ std::filesystem::path(executablePath).parent_path() };
    
    //nlohmann::json json{};
    //KtSerializer::Deserialize(json, executableDirectory / "config.json");

    rootPath_ = ROOT_DIRECTORY;
    frameworkPath_ = rootPath_ / "Framework";
    enginePath_ = rootPath_ / "Engine";

    //projectPath_ = json.at("projectPath").get<std::string>();
    projectPath_ = rootPath_ / "TestApplication";
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
