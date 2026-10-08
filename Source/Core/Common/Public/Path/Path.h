#pragma once
#include "types.h"
#include <filesystem>
#include <string>
#include <string_view>

class UPath final
{
private:
	friend struct std::hash<UPath>;

public:
	UPath() = default;
	UPath(UPath&& path) noexcept;
	UPath(UPath const& path);
	explicit UPath(std::string&& source) noexcept;
	explicit UPath(std::string const& source);
	explicit UPath(char const* source);
	UPath(std::filesystem::path const& source);

	auto operator=(UPath const& other) -> UPath&;
	auto operator=(UPath&& other) noexcept -> UPath&;

	auto Directory() const -> UPath;
	auto Name() const -> std::string;
	auto Extension() const -> std::string;
	auto Stem() const -> std::string;
	auto HasDirectory() const -> b8;
	auto IsEmpty() const -> b8;
	auto IsFile() const -> b8;
	auto Exists() const -> b8;

	auto ToString() const -> std::string const&;
	auto ToPath() const -> std::filesystem::path;
	
	/// Remove file or directory
	void Remove() const;

	operator std::string() const;
	operator std::filesystem::path() const;
	operator b8() const;

	auto operator/(std::string_view string) const -> UPath;
	auto operator/(UPath const& path) const -> UPath;
	
	auto operator+(char const* string) const -> UPath;
	auto operator+(std::string_view string) const -> UPath;

	auto operator==(UPath const& other) const noexcept -> b8;

private:
	std::string source_;

	static std::string_view enginePath_;
	static std::string_view projectPath_;
};

template<>
struct std::hash<UPath>
{
	::size operator()(UPath const& p) const noexcept;
};
