#pragma once
#include <array>
#include <kotono_common/types.h>
#include <kotono_io/serialize_base.h>
#include <string>
class UGuid final
{
private:
	friend struct std::hash<UGuid>;

public:
	UGuid();
	UGuid(std::string_view string);

	std::string ToString() const;

	operator std::string() const;
	UGuid& operator=(std::string_view string);

	bool operator==(UGuid const& other) const noexcept;

private:
	void ParseHex(std::string_view string, size str_pos, size num_bytes, size array_offset);

private:
	std::array<u8, 16> bytes_;
};

template<>
struct std::hash<UGuid>
{
	::size operator()(UGuid const& g) const noexcept;
};

template <>
struct USerialize<UGuid>
{
	void operator()(nlohmann::json& json, UGuid const& v) const;
};

template <>
struct UDeserialize<UGuid>
{
	void operator()(nlohmann::json const& json, UGuid& v) const;
};
