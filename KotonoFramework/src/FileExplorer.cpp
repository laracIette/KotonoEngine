#include "FileExplorer.h"
#include "Framework.h"

KtFileExplorer::KtFileExplorer() :
    directoryPath_(Framework.GetPath().GetSolutionPath())
{
}

const std::filesystem::path& KtFileExplorer::GetDirectoryPath() const
{
    return directoryPath_;
}

void KtFileExplorer::SetDirectoryPath(const std::filesystem::path& directoryPath)
{
    directoryPath_ = directoryPath;
}

std::vector<std::filesystem::path> KtFileExplorer::GetDirectories() const
{
    std::vector<std::filesystem::path> directories;
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(directoryPath_))
    {
        if (entry.is_directory())
        {
            directories.push_back(entry.path());
        }
    }
    return directories;
}

std::vector<KtFile> KtFileExplorer::Find(const std::string& name) const
{
    std::vector<KtFile> files;
    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(directoryPath_))
    {
        if (entry.is_regular_file() && entry.path().filename().string().find(name) != std::string::npos)
        {
            files.push_back(KtFile(entry.path()));
        }
    }
    return files;
}

std::vector<KtFile> KtFileExplorer::GetFiles(const ConditionnalFunction& condition) const
{
    std::vector<KtFile> files;
    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(directoryPath_))
    {
        if (entry.is_regular_file() && condition(entry.path()))
        {
            files.push_back(KtFile(entry.path()));
        }
    }
    return files;
}
