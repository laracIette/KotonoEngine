#pragma once
#include <string>
#include <filesystem>
#include "types.h"
class UPath final
{
private:
	friend struct std::hash<UPath>;

public:
	UPath();
	UPath(const std::string& source);
	UPath(std::string&& source);
	UPath(const char* source);
	UPath(const std::filesystem::path& source);

	std::filesystem::path Directory() const;
	std::string Name() const;
	std::string Extension() const;
	std::string Stem() const;
	bool IsFile() const;

	std::string ToString() const;
	std::filesystem::path ToPath() const;

	operator std::string() const;
	operator std::filesystem::path() const;

	bool operator==(const UPath& other) const noexcept;

	friend UPath operator/(const UPath& r, const UPath& l);

private:
	std::string source_;
};

template<>
struct std::hash<UPath>
{
	::size operator()(const UPath& p) const noexcept;
};
