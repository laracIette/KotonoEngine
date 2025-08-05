#include "FileExplorer.h"
#include "Framework.h"

KtFileExplorer::KtFileExplorer() :
    _directoryPath(Framework.GetPath().GetSolutionPath())
{
}

void KtFileExplorer::SetDirectoryPath(const std::filesystem::path& directoryPath)
{
    _directoryPath = directoryPath;
}

const std::vector<std::filesystem::path> KtFileExplorer::GetDirectories() const
{
    std::vector<std::filesystem::path> directories;
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(_directoryPath))
    {
        if (entry.is_directory())
        {
            directories.push_back(entry.path());
        }
    }
    return directories;
}

const std::vector<KtFile> KtFileExplorer::Find(const std::string& name) const
{
    std::vector<KtFile> files;
    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(_directoryPath))
    {
        if (entry.is_regular_file() && entry.path().filename().string().find(name) != std::string::npos)
        {
            files.push_back(KtFile(entry.path()));
        }
    }
    return files;
}

const std::vector<KtFile> KtFileExplorer::GetFiles(const ConditionnalFunction& condition) const
{
    std::vector<KtFile> files;
    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(_directoryPath))
    {
        if (entry.is_regular_file() && condition(entry.path()))
        {
            files.push_back(KtFile(entry.path()));
        }
    }
    return files;
}
