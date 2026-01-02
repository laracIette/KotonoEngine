#pragma once
#include <string>
#include <array>
#include <kotono_common/types.h>
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

	bool operator==(const UGuid& other) const noexcept;

private:
	std::array<u8, 16> bytes_;
};

template<>
struct std::hash<UGuid>
{
	::size operator()(const UGuid& g) const noexcept;
};
