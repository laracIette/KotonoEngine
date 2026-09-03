#include "Guid.h"
#include <format>
#include <kotono_common/hash_utils.h>
#include <nlohmann/json.hpp>
#include <random>
#include <sstream>

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

UGuid::UGuid(const std::string& string)
{
    if (string.length() < 36)
    {
        return;
    }

    // Helper to parse a specific hex range into bytes
    auto parse_hex = [&](size str_pos, size num_bytes, size array_offset)
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

    parse_hex(0, 4, 0);   
    parse_hex(9, 2, 4);   
    parse_hex(14, 2, 6);  
    parse_hex(19, 2, 8);  
    parse_hex(24, 6, 10); 
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

UGuid& UGuid::operator=(const std::string& string)
{
    if (string.length() < 36)
    {
        return *this;
    }

    // Helper to parse a specific hex range into bytes
    auto parse_hex = [&](size str_pos, size num_bytes, size array_offset)
        {
            for (size i{ 0 }; i < num_bytes; ++i)
            {
                // Each byte is 2 hex chars
                std::from_chars(
                    string.data() + str_pos + (i * 2),
                    string.data() + str_pos + (i * 2) + 2,
                    bytes_[array_offset + i], 16
                );
            }
        };

    // Parse according to 8-4-4-4-12 structure
    parse_hex(0, 4, 0);   // First 8 chars -> bytes 0-3
    parse_hex(9, 2, 4);   // Next 4 chars  -> bytes 4-5
    parse_hex(14, 2, 6);  // Next 4 chars  -> bytes 6-7
    parse_hex(19, 2, 8);  // Next 4 chars  -> bytes 8-9
    parse_hex(24, 6, 10); // Last 12 chars -> bytes 10-15

    return *this;
}

bool UGuid::operator==(const UGuid& other) const noexcept
{
    auto [a_low, a_high] { std::bit_cast<std::array<u64, 2>>(bytes_) };
    auto [b_low, b_high] { std::bit_cast<std::array<u64, 2>>(other.bytes_) };

    return (a_low == b_low) && (a_high == b_high);
}

size std::hash<UGuid>::operator()(const UGuid& g) const noexcept
{
    auto [low, high] { std::bit_cast<std::array<u64, 2>>(g.bytes_) };
    return static_cast<::size>(low ^ high);
}

void USerialize<UGuid>::operator()(nlohmann::json& json, const UGuid& v) const
{
    USerialize<std::string>{}(json, v.ToString());
}

void UDeserialize<UGuid>::operator()(const nlohmann::json& json, UGuid& v) const
{
    const auto string{ json.get<std::string>() };
    if (!string.empty())
    {
        v = json;
    }
}
