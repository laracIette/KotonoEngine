#pragma once
#include <string>
#include <filesystem>
class UPath final
{
private:
	friend struct std::hash<UPath>;

public:
	UPath();
	UPath(const std::string& source);
	UPath(std::string&& source);
	UPath(const char* source);

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
	size_t operator()(const UPath& p) const noexcept;
};
