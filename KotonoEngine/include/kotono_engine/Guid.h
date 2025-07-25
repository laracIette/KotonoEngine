#pragma once
#include <string>
#include <array>
class UGuid final
{
public:
	UGuid();

	operator std::string() const;
	void operator=(const std::string& string);

	bool operator==(const UGuid& other) const;

private:
	std::array<uint64_t, 4> _data;
};

