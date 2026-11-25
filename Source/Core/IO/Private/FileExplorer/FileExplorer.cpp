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

auto UFileExplorer::GetDirectories() const -> UList<UPath>
{
    UList<UPath> directories;
    for (auto const& entry : std::filesystem::directory_iterator(directoryPath_.ToPath()))
    {
        if (entry.is_directory())
        {
            directories.Add(entry.path());
        }
    }
    return directories;
}

auto UFileExplorer::Find(const std::string& name) const -> UList<UFile>
{
    UList<UFile> files;
    for (auto const& entry : std::filesystem::recursive_directory_iterator(directoryPath_.ToPath()))
    {
        if (entry.is_regular_file() && entry.path().filename().string().find(name) != std::string::npos)
        {
            files.Add(UFile{ entry.path() });
        }
    }
    return files;
}

auto UFileExplorer::GetFiles(b8 isRecursive) const -> UList<UFile>
{
    UList<std::filesystem::directory_entry> entries{};
    if (isRecursive)
    {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath_.ToPath()))
        {
            entries.Add(entry);
        }
    }
    else
    {
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath_.ToPath()))
        {
            entries.Add(entry);
        }
    }

    UList<UFile> files;
    for (const auto& entry : entries)
    {
        if (entry.is_regular_file())
        {
            files.Add(UFile{ entry.path() });
        }
    }
    return files;
}
