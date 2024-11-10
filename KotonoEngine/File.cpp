#include "File.h"
#include <fstream>
#include <sstream>
#include <iostream>

File::File(const std::filesystem::path& path) :
    _path(path)
{
}

const std::filesystem::path& File::GetPath() const
{
    return _path;
}

const std::filesystem::path File::GetDirectory() const
{
    return _path.parent_path();
}

const std::string File::GetName() const
{
    return _path.filename().string();
}

const std::string File::GetExtension() const
{
    return _path.extension().string();
}

const std::string File::GetStem() const
{
    return _path.stem().string();
}

const std::string File::GetContent() const
{
    // Check if path exists.
    if (!std::filesystem::exists(_path))
    {
        std::cerr << "Failed to find a file at '" << _path << '\'' << std::endl;
        return "";
    }

    // Open file
    std::ifstream file(_path);
    
    if (!file.is_open())
    {
        std::cerr << "Failed to open the file at '" << _path << '\'' << std::endl;
        return "";
    }

    // Read file
    std::ostringstream fileContents;
    fileContents << file.rdbuf();
    return fileContents.str();
}
