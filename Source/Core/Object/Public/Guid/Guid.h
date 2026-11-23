#pragma once
#include <array>
#include <types.h>
#include <serialize_base/serialize_base.h>
#include <string>

class UGuid final
{
private:
	friend struct std::hash<UGuid>;

public:
	UGuid();
	explicit UGuid(std::string_view string);

	auto ToString() const -> std::string;

	auto operator=(std::string_view string) -> UGuid&;

	auto operator==(UGuid const& other) const noexcept -> b8;

private:
	void ParseHex(std::string_view string, size str_pos, size num_bytes, size array_offset);

private:
	std::array<u8, 16> bytes_;
};

template<>
struct std::hash<UGuid>
{
	auto operator()(UGuid const& g) const noexcept -> ::size;
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
