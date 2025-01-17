#include "FileExplorer.h"
#include "KotonoEngine.h"

FileExplorer::FileExplorer() :
    _directoryPath(Engine.GetProjectDirectory())
{
}

void FileExplorer::SetDirectoryPath(const std::string& directoryPath)
{
    _directoryPath = directoryPath;
}

const std::vector<std::filesystem::path> FileExplorer::GetDirectories() const
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

template <class T>
const std::vector<File> FileExplorer::GetFilesOfType() const
{
    std::vector<File> files;
    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(_directoryPath))
    {
        if (!entry.is_regular_file())
        {
            continue;
        }

        File file(entry.path());

        if (file.IsOfType<T>())
        {
            files.push_back(file);
        }
    }
    return files;
}

const std::vector<File> FileExplorer::Find(const std::string& name) const
{
    std::vector<File> files;
    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(_directoryPath))
    {
        if (entry.is_regular_file() && entry.path().filename().string().find(name) != std::string::npos)
        {
            files.push_back(File(entry.path()));
        }
    }
    return files;
}
