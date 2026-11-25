#pragma once

#include "Containers/List.h"
#include "Containers/Span.h"
#include "Path/Path.h"
#include "types.h"

#include <chrono>
#include <filesystem>
#include <fstream>

class UFile final
{
public:
    explicit UFile(UPath&& path);
    explicit UFile(UPath const& path);

	/// Get whether the file exists
	auto Exists() const -> b8;

	/// Get the path to the file
	auto Path() const -> UPath const&;

	/// Get the path to the directory of the file
	auto Directory() const -> UPath;

	/// Get the name of the file with extension
	auto Name() const -> std::string;

	/// Get the extension of the file
	auto Extension() const -> std::string;

	/// Get the name of the file without extension
	auto Stem() const -> std::string;

	/// Get the last write time of the file
	auto LastWriteTime() const -> std::chrono::file_clock::time_point;

	/// Get the content of the file, for text files
	auto ReadString() const -> std::string;

	/// Get the content of the file, for binary files
	auto ReadBinary() const -> UList<u8>;

	/// Set the content of the file, for text files
	void WriteString(std::string_view data) const;

	/// Set the content of the file, for binary files
	void WriteBinary(USpan<u32 const> data) const;

private:
	/// Create the file's directory if it doesn't exist, then return the opened file
	auto OpenFile() const -> std::ofstream;

private:
	UPath path_;
};

