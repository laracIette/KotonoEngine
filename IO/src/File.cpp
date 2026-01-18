#include "File.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <kotono_common/log.h>

#define KT_LOG_IMPORTANCE_LEVEL_FILE ELogImportanceLevel::High

UFile::UFile(const std::filesystem::path& path) :
    path_(path)
{
}

bool UFile::Exists() const
{
    return std::filesystem::exists(path_);
}

const std::filesystem::path& UFile::Path() const
{
    return path_;
}

std::filesystem::path UFile::Directory() const
{
    return path_.parent_path();
}

std::string UFile::Name() const
{
    return path_.filename().string();
}

std::string UFile::Extension() const
{
    return path_.extension().string();
}

std::string UFile::Stem() const
{
    return path_.stem().string();
}

std::chrono::file_clock::time_point UFile::LastWriteTime() const
{
	return std::filesystem::last_write_time(path_);
}

std::string UFile::ReadString() const
{
    // Check if path exists.
    if (!Exists())
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_FILE, "IO", "Failed to find a file at {}", path_.string());
        return "";
    }

    // Open file
    std::ifstream file(path_);

    if (!file.is_open())
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_FILE, "IO", "Failed to open the file at {}", path_.string());
        return "";
    }

    // Read file
    std::ostringstream fileContents;
    fileContents << file.rdbuf();

    file.close();

    return fileContents.str();
}

std::vector<u8> UFile::ReadBinary() const
{
    if (!Exists())
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_FILE, "IO", "Failed to find a file at {}", path_.string());
        return {};
    }

    std::ifstream file(path_, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_FILE, "IO", "Failed to open the file at {}", path_.string());
        return {};
    }

    const size fileSize = static_cast<size>(file.tellg());
    std::vector<u8> buffer(fileSize);

    file.seekg(0);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

    file.close();

    return buffer;
}

void UFile::WriteString(const std::string_view data) const
{
    // Open file for writing
    std::ofstream file(path_, std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_FILE, "IO", "Failed to open the file at {}", path_.string());
        return;
    }

    // Write data to file
    file << data;

    // Close file
    file.close();
}

void UFile::WriteBinary(const std::span<u32> data) const
{
    // Open file for writing in binary mode
    std::ofstream file(path_, std::ios::out | std::ios::binary | std::ios::trunc);

    if (!file.is_open())
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_FILE, "IO", "Failed to open the file at {}", path_.string());
        return;
    }

    // Write data to file as binary
    file.write(reinterpret_cast<const char*>(data.data()), data.size_bytes());

    // Close file
    file.close();
}
