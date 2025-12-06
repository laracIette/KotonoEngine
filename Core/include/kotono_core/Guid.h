#pragma once
#include <string>
#include <array>
#include <kotono_common/hash_utils.h>
struct UGuid final
{
	friend struct std::hash<UGuid>;

public:
	UGuid();

	operator std::string() const;
	void operator=(const std::string& string);

	bool operator==(const UGuid& other) const;

	std::string ToString() const;

private:
	std::array<uint64_t, 4> data_;
};

template<>
struct std::hash<UGuid>
{
	size_t operator()(const UGuid& g) const noexcept
	{
		size_t h{ 0 };
		combine(h, std::hash<uint64_t>{}(g.data_[0]));
		combine(h, std::hash<uint64_t>{}(g.data_[1]));
		combine(h, std::hash<uint64_t>{}(g.data_[2]));
		combine(h, std::hash<uint64_t>{}(g.data_[3]));
		return h;
	}
};

