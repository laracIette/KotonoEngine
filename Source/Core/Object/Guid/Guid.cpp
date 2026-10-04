#include "Guid.h"
#include <bit>
#include <format>
#include <nlohmann/json.hpp>
#include <random>

UGuid::UGuid()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<u32> dist(0, 255);

    for (u8& byte : bytes_) 
    { 
        byte = static_cast<u8>(dist(gen)); 
    }

    // Version 4: Set the 7th byte's high nibble to 4
    bytes_[6] = (bytes_[6] & 0x0F) | 0x40;

    // Variant 1 (RFC 4122): Set the 9th byte's high two bits to 10
    bytes_[8] = (bytes_[8] & 0x3F) | 0x80;
}

UGuid::UGuid(std::string_view string)
{
    if (string.length() < 36)
    {
        return;
    }

    // Parse according to 8-4-4-4-12 structure
    ParseHex(string, 0, 4, 0);   // First 8 chars -> bytes 0-3
    ParseHex(string, 9, 2, 4);   // Next 4 chars  -> bytes 4-5
    ParseHex(string, 14, 2, 6);  // Next 4 chars  -> bytes 6-7
    ParseHex(string, 19, 2, 8);  // Next 4 chars  -> bytes 8-9
    ParseHex(string, 24, 6, 10); // Last 12 chars -> bytes 10-15
}

std::string UGuid::ToString() const
{
    return std::format(
        "{:02x}{:02x}{:02x}{:02x}-{:02x}{:02x}-{:02x}{:02x}-{:02x}{:02x}-{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}",
        bytes_[0],  bytes_[1],  bytes_[2],  bytes_[3],
        bytes_[4],  bytes_[5],
        bytes_[6],  bytes_[7],
        bytes_[8],  bytes_[9],
        bytes_[10], bytes_[11], bytes_[12], bytes_[13], bytes_[14], bytes_[15]
    );
}

UGuid::operator std::string() const
{
    return ToString();
}

UGuid& UGuid::operator=(std::string_view string)
{
    if (string.length() < 36)
    {
        return *this;
    }

    ParseHex(string, 0, 4, 0);  
    ParseHex(string, 9, 2, 4);  
    ParseHex(string, 14, 2, 6); 
    ParseHex(string, 19, 2, 8); 
    ParseHex(string, 24, 6, 10);

    return *this;
}

bool UGuid::operator==(UGuid const& other) const noexcept
{
    auto const [a_low, a_high] { std::bit_cast<std::array<u64, 2>>(bytes_) };
    auto const [b_low, b_high] { std::bit_cast<std::array<u64, 2>>(other.bytes_) };

    return (a_low == b_low) && (a_high == b_high);
}

void UGuid::ParseHex(std::string_view string, size str_pos, size num_bytes, size array_offset)
{
    for (size i{ 0 }; i < num_bytes; ++i)
    {
        std::from_chars(
            string.data() + str_pos + (i * 2),
            string.data() + str_pos + (i * 2) + 2,
            bytes_[array_offset + i], 16
        );
    }
};

size std::hash<UGuid>::operator()(UGuid const& g) const noexcept
{
    auto const [low, high] { std::bit_cast<std::array<u64, 2>>(g.bytes_) };
    return static_cast<::size>(low ^ high);
}

void USerialize<UGuid>::operator()(nlohmann::json& json, UGuid const& v) const
{
    USerialize<std::string>{}(json, v.ToString());
}

void UDeserialize<UGuid>::operator()(nlohmann::json const& json, UGuid& v) const
{
    auto const string{ json.get<std::string>() };
    if (!string.empty())
    {
        v = json;
    }
}
