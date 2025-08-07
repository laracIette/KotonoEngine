#include "File.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "log.h"

#define KT_LOG_IMPORTANCE_LEVEL_FILE KT_LOG_IMPORTANCE_LEVEL_HIGH

KtFile::KtFile(const std::filesystem::path& path) :
    path_(path)
{
}

bool KtFile::Exists() const
{
    return std::filesystem::exists(path_);
}

const std::filesystem::path& KtFile::GetPath() const
{
    return path_;
}

std::filesystem::path KtFile::GetDirectory() const
{
    return path_.parent_path();
}

std::string KtFile::GetName() const
{
    return path_.filename().string();
}

std::string KtFile::GetExtension() const
{
    return path_.extension().string();
}

std::string KtFile::GetStem() const
{
    return path_.stem().string();
}

std::string KtFile::ReadString() const
{
    // Check if path exists.
    if (!Exists())
    {
        KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_FILE, "Failed to find a file at '%s'", path_.string().c_str());
        return "";
    }

    // Open file
    std::ifstream file(path_);

    if (!file.is_open())
    {
        KT_LOG_KF(KT_LOG_IMPORTANCE_LEVEL_FILE, "Failed to open the file at '%s'", path_.string().c_str());
        return "";
    }

    // Read file
    std::ostringstream fileContents;
    fileContents << file.rdbuf();

    file.close();

    return fileContents.str();
}

std::vector<uint8_t> KtFile::ReadBinary() const
{
    if (!std::filesystem::exists(path_))
    {
        std::cerr << "Failed to find a file at '" << path_ << '\'' << std::endl;
        return {};
    }

    std::ifstream file(path_, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "Failed to open the file at '" << path_ << '\'' << std::endl;
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
    std::ofstream file(path_, std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
        std::cerr << "Failed to open the file at '" << path_ << "' for writing." << std::endl;
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
    std::ofstream file(path_, std::ios::out | std::ios::binary | std::ios::trunc);

    if (!file.is_open())
    {
        std::cerr << "Failed to open the file at '" << path_ << "' for writing." << std::endl;
        return;
    }

    // Write data to file as binary
    file.write(reinterpret_cast<const char*>(data.data()), data.size_bytes());

    // Close file
    file.close();
}
