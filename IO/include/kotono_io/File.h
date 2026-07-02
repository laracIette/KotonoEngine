#pragma once
#include <kotono_common/Path.h>
#include <kotono_common/types.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <span>
class UFile final
{
public:
	UFile(const UPath& path);
	UFile(UPath&& path);

	/// Get whether the file exists
	bool Exists() const;
	/// Get the path to the file
	const UPath& Path() const;
	/// Get the path to the directory of the file
	std::filesystem::path Directory() const;
	/// Get the name of the file with extension
	std::string Name() const;
	/// Get the extension of the file
	std::string Extension() const;
	/// Get the name of the file without extension
	std::string Stem() const;
	/// Get the last write time of the file
	std::chrono::file_clock::time_point LastWriteTime() const;
	/// Get the content of the file, for text files
	std::string ReadString() const;
	/// Get the content of the file, for binary files
	std::vector<u8> ReadBinary() const;
	/// Set the content of the file, for text files
	void WriteString(const std::string_view data) const;
	/// Set the content of the file, for binary files
	void WriteBinary(const std::span<u32> data) const;

private:
	/// Create the file's directory if it doesn't exist, then return the opened file
	std::ofstream OpenFile() const;

private:
	const UPath path_;
};

