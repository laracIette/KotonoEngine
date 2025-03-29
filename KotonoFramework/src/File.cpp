#include "File.h"
#include <fstream>
#include <sstream>
#include <iostream>

KtFile::KtFile(const std::filesystem::path& path) :
    _path(path)
{
}

const std::filesystem::path& KtFile::GetPath() const
{
    return _path;
}

const std::filesystem::path KtFile::GetDirectory() const
{
    return _path.parent_path();
}

const std::string KtFile::GetName() const
{
    return _path.filename().string();
}

const std::string KtFile::GetExtension() const
{
    return _path.extension().string();
}

const std::string KtFile::GetStem() const
{
    return _path.stem().string();
}

const std::string KtFile::ReadString() const
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

    file.close();

    return fileContents.str();
}

const std::vector<uint8_t> KtFile::ReadBinary() const
{
    if (!std::filesystem::exists(_path))
    {
        std::cerr << "Failed to find a file at '" << _path << '\'' << std::endl;
        return {};
    }

    std::ifstream file(_path, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "Failed to open the file at '" << _path << '\'' << std::endl;
        return {};
    }

    const size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<uint8_t> buffer(fileSize);

    file.seekg(0);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

    file.close();

    return buffer;
}

void KtFile::WriteString(const std::string_view data) const
{
    // Open file for writing
    std::ofstream file(_path, std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
        std::cerr << "Failed to open the file at '" << _path << "' for writing." << std::endl;
        return;
    }

    // Write data to file
    file << data;

    // Close file
    file.close();
}

void KtFile::WriteBinary(const std::span<uint32_t> data) const
{
    // Open file for writing in binary mode
    std::ofstream file(_path, std::ios::out | std::ios::binary | std::ios::trunc);

    if (!file.is_open())
    {
        std::cerr << "Failed to open the file at '" << _path << "' for writing." << std::endl;
        return;
    }

    // Write data to file as binary
    file.write(reinterpret_cast<const char*>(data.data()), data.size_bytes());

    // Close file
    file.close();
}
