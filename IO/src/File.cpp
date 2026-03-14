#include "File.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <kotono_common/log.h>

#define KT_LOG_IMPORTANCE_LEVEL_FILE ELogImportanceLevel::High

UFile::UFile(const UPath& path) 
    : path_(path)
{
}

UFile::UFile(UPath&& path) 
    : path_(std::move(path))
{
}

bool UFile::Exists() const
{
    return std::filesystem::exists(path_.ToPath());
}

const UPath& UFile::Path() const
{
    return path_;
}

std::filesystem::path UFile::Directory() const
{
    return path_.Directory();
}

std::string UFile::Name() const
{
    return path_.Name();
}

std::string UFile::Extension() const
{
    return path_.Extension();
}

std::string UFile::Stem() const
{
    return path_.Stem();
}

std::chrono::file_clock::time_point UFile::LastWriteTime() const
{
	return std::filesystem::last_write_time(path_.ToPath());
}

std::string UFile::ReadString() const
{
    // Check if path exists.
    if (!Exists())
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_FILE, "IO", "Failed to find a file at {}", path_.ToString());
        return "";
    }

    // Open file
    std::ifstream file(path_.ToPath());

    if (!file.is_open())
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_FILE, "IO", "Failed to open the file at {}", path_.ToString());
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
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_FILE, "IO", "Failed to find a file at {}", path_.ToString());
        return {};
    }

    std::ifstream file(path_.ToPath(), std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_FILE, "IO", "Failed to open the file at {}", path_.ToString());
        return {};
    }

    const size fileSize{ static_cast<size>(file.tellg()) };
    std::vector<u8> buffer(fileSize);

    file.seekg(0);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

    file.close();

    return buffer;
}

void UFile::WriteString(const std::string_view data) const
{
    // Open file for writing
    std::ofstream file(path_.ToPath(), std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_FILE, "IO", "Failed to open the file at {}", path_.ToString());
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
    std::ofstream file(path_.ToPath(), std::ios::out | std::ios::binary | std::ios::trunc);

    if (!file.is_open())
    {
        KT_LOG(KT_LOG_IMPORTANCE_LEVEL_FILE, "IO", "Failed to open the file at {}", path_.ToString());
        return;
    }

    // Write data to file as binary
    file.write(reinterpret_cast<const char*>(data.data()), data.size_bytes());

    // Close file
    file.close();
}
