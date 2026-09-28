#pragma once
#include "types.h"
#include <concepts>
#include <filesystem>
#include <string>
class UPath final
{
private:
	friend struct std::hash<UPath>;

public:
	UPath();
	UPath(std::string const& source);
	UPath(std::string&& source);
	UPath(char const* source);
	UPath(UPath const& path);
	UPath(UPath&& path);
	UPath(std::filesystem::path const& source);

	auto Directory() const -> UPath;
	auto Name() const -> std::string;
	auto Extension() const -> std::string;
	auto Stem() const -> std::string;
	auto IsEmpty() const -> b8;
	auto IsFile() const -> b8;
	auto Exists() const -> b8;

	auto ToString() const -> std::string const&;
	auto ToPath() const -> std::filesystem::path;

	UPath& operator=(UPath const& other);
	UPath& operator=(UPath&& other);

	operator std::string() const;
	operator std::filesystem::path() const;
	operator b8() const;

	b8 operator==(UPath const& other) const noexcept;

	friend UPath operator/(UPath const& r, UPath const& l);

private:
	std::string source_;
};

template<>
struct std::hash<UPath>
{
	::size operator()(UPath const& p) const noexcept;
};
