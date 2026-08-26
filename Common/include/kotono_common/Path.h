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
	UPath(std::string const& source = "");
	UPath(std::string&& source);
	UPath(char const* source);
	UPath(std::filesystem::path const& source);

	UPath Directory() const;
	std::string Name() const;
	std::string Extension() const;
	std::string Stem() const;
	b8 IsEmpty() const;
	b8 IsFile() const;

	std::string const& ToString() const;
	std::filesystem::path ToPath() const;

	operator std::string() const;
	operator std::filesystem::path() const;
	operator bool() const;

	bool operator==(UPath const& other) const noexcept;

	friend UPath operator/(UPath const& r, UPath const& l);

private:
	std::string source_;
};

template<>
struct std::hash<UPath>
{
	::size operator()(UPath const& p) const noexcept;
};
