#pragma once
#include <string>
#include <array>
class UGuid final
{
private:
	friend struct std::hash<UGuid>;

public:
	UGuid();
	UGuid(const std::string& string);

	std::string ToString() const;

	operator std::string() const;
	UGuid& operator=(const std::string& string);

	bool operator==(const UGuid& other) const;

private:
	std::array<uint64_t, 4> data_;
};

template<>
struct std::hash<UGuid>
{
	size_t operator()(const UGuid& g) const noexcept;
};
