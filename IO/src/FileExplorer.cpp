#include "FileExplorer.h"
#include "File.h"

UFileExplorer::UFileExplorer() :
    directoryPath_(ENGINE_DIRECTORY)
{
}

UFileExplorer::UFileExplorer(const UPath& path) :
    directoryPath_(path)
{
}

const UPath& UFileExplorer::GetDirectoryPath() const
{
    return directoryPath_;
}

void UFileExplorer::SetDirectoryPath(const UPath& directoryPath)
{
    directoryPath_ = directoryPath;
}

std::vector<UPath> UFileExplorer::GetDirectories() const
{
    std::vector<UPath> directories;
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(directoryPath_))
    {
        if (entry.is_directory())
        {
            directories.push_back(entry.path().string());
        }
    }
    return directories;
}

std::vector<UFile> UFileExplorer::Find(const std::string& name) const
{
    std::vector<UFile> files;
    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(directoryPath_))
    {
        if (entry.is_regular_file() && entry.path().filename().string().find(name) != std::string::npos)
        {
            files.push_back(UFile(entry.path()));
        }
    }
    return files;
}

std::vector<UFile> UFileExplorer::GetFiles(const bool isRecursive) const
{
    std::vector<std::filesystem::directory_entry> entries{};
    if (isRecursive)
    {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath_))
        {
            entries.push_back(entry);
        }
    }
    else
    {
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath_))
        {
            entries.push_back(entry);
        }
    }

    std::vector<UFile> files;
    for (const auto& entry : entries)
    {
        if (entry.is_regular_file())
        {
            files.push_back(UFile(entry.path()));
        }
    }
    return files;
}
