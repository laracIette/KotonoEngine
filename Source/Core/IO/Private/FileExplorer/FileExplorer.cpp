#include "FileExplorer/FileExplorer.h"

#include "File/File.h"

UFileExplorer::UFileExplorer()
    : directoryPath_{ ENGINE_DIRECTORY }
{
}

UFileExplorer::UFileExplorer(UPath&& path)
    : directoryPath_{ Move(path) }
{
}

UFileExplorer::UFileExplorer(UPath const& path)
    : directoryPath_{ path }
{
}

auto UFileExplorer::GetDirectoryPath() const -> UPath const&
{
    return directoryPath_;
}

void UFileExplorer::SetDirectoryPath(UPath const& directoryPath)
{
    directoryPath_ = directoryPath;
}

auto UFileExplorer::GetDirectories() const -> std::vector<UPath>
{
    std::vector<UPath> directories;
    for (auto const& entry : std::filesystem::directory_iterator(directoryPath_.ToPath()))
    {
        if (entry.is_directory())
        {
            directories.emplace_back(entry.path());
        }
    }
    return directories;
}

auto UFileExplorer::Find(const std::string& name) const -> std::vector<UFile>
{
    std::vector<UFile> files;
    for (auto const& entry : std::filesystem::recursive_directory_iterator(directoryPath_.ToPath()))
    {
        if (entry.is_regular_file() && entry.path().filename().string().find(name) != std::string::npos)
        {
            files.emplace_back(entry.path());
        }
    }
    return files;
}

auto UFileExplorer::GetFiles(b8 isRecursive) const -> std::vector<UFile>
{
    std::vector<std::filesystem::directory_entry> entries{};
    if (isRecursive)
    {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath_.ToPath()))
        {
            entries.push_back(entry);
        }
    }
    else
    {
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath_.ToPath()))
        {
            entries.push_back(entry);
        }
    }

    std::vector<UFile> files;
    for (const auto& entry : entries)
    {
        if (entry.is_regular_file())
        {
            files.emplace_back(entry.path());
        }
    }
    return files;
}
