#include "File/File.h"

#include "Logging/log.h"

#include <iostream>
#include <sstream>

UFile::UFile(UPath const& path)
    : path_{ path }
{
}

UFile::UFile(UPath&& path) 
    : path_{ std::move(path) }
{
}

auto UFile::Exists() const -> b8
{
    return path_.Exists();
}

auto UFile::Path() const -> UPath const&
{
    return path_;
}

auto UFile::Directory() const -> UPath
{
    return path_.Directory();
}

auto UFile::Name() const -> std::string
{
    return path_.Name();
}

auto UFile::Extension() const -> std::string
{
    return path_.Extension();
}

auto UFile::Stem() const -> std::string
{
    return path_.Stem();
}

auto UFile::LastWriteTime() const -> std::chrono::file_clock::time_point
{
	return std::filesystem::last_write_time(path_.ToPath());
}

auto UFile::ReadString() const -> std::string
{
    // Check if path exists.
    if (!Exists())
    {
        Log(Normal, "Failed to find a file at {0}", path_.ToString());
        return "";
    }

    // Open file
    std::ifstream file(path_.ToPath());

    if (!file.is_open())
    {
        Log(Normal, "Failed to open the file at {0}", path_.ToString());
        return "";
    }

    // Read file
    std::ostringstream fileContents;
    fileContents << file.rdbuf();

    file.close();

    return fileContents.str();
}

auto UFile::ReadBinary() const -> std::vector<u8>
{
    if (!Exists())
    {
        Log(Normal, "Failed to find a file at {0}", path_.ToString());
        return {};
    }

    std::ifstream file(path_.ToPath(), std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        Log(Normal, "Failed to open the file at {0}", path_.ToString());
        return {};
    }

    size const fileSize{ static_cast<size>(file.tellg()) };
    std::vector<u8> buffer(fileSize);

    file.seekg(0);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

    file.close();

    return buffer;
}

void UFile::WriteString(std::string_view data) const
{
    // Open file for writing
    auto file{ OpenFile() };

    if (!file.is_open())
    {
        Log(Normal, "Failed to open the file at {0}", path_.ToString());
        return;
    }

    // Write data to file
    file << data;

    // Close file
    file.close();
}

void UFile::WriteBinary(USpan<u32 const> data) const
{
    // Open file for writing in binary mode
    auto file{ OpenFile() };

    if (!file.is_open())
    {
        Log(Normal, "Failed to open the file at {0}", path_.ToString());
        return;
    }

    // Write data to file as binary
    file.write(reinterpret_cast<const char*>(data.Data()), data.SizeBytes());

    // Close file
    file.close();
}

auto UFile::OpenFile() const -> std::ofstream
{
    auto const parentDir{ path_.ToPath().parent_path() };

    // Create the directories if they don't exist
    if (!parentDir.empty())
    {
        std::filesystem::create_directories(parentDir);
    }

    return std::ofstream{ path_.ToPath(), std::ios::out | std::ios::binary | std::ios::trunc };
}
