#include "FileExplorer.h"

FileExplorer::FileExplorer() :
    _directoryPath("")
{
}

void FileExplorer::SetDirectoryPath(const std::string& directoryPath)
{
    _directoryPath = directoryPath;
}

const std::vector<std::string> FileExplorer::GetDirectories() const
{
    std::vector<std::string> directories;
    for (const std::filesystem::directory_entry& entry : GetDirectoryOperator())
    {
        if (entry.is_directory())
        {
            directories.push_back(entry.path().string());
        }
    }
    return directories;
}

const std::vector<File> FileExplorer::GetFiles() const
{
    std::vector<File> files;
    for (const std::filesystem::directory_entry& entry : GetDirectoryOperator())
    {
        if (entry.is_regular_file())
        {
            files.push_back(File(entry.path()));
        }
    }
    return files;
}

const std::vector<File> FileExplorer::Find(const std::string& name) const
{
    std::vector<File> files;
    for (const File file : GetFiles())
    {
        if (file.GetName().find(name) != std::string::npos)
        {
            files.push_back(file);
        }
    }
    return files;
}

const std::filesystem::directory_iterator FileExplorer::GetDirectoryOperator() const
{
    return std::filesystem::directory_iterator(_directoryPath);
}
