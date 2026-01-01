#include "Guid.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <kotono_common/hash_utils.h>

UGuid::UGuid()
{
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<u64> dist;

    for (auto& part : data_)
    {
        part = dist(gen);
    }

    // Set the version bits (4 bits for GUID v4)
    data_[0] &= 0xFFFFFFFFFFFFFF0F; // Clear version bits
    data_[0] |= 0x0000000000000040; // Set version 4 (random GUID)
}

UGuid::UGuid(const std::string& string)
{
    std::istringstream iss(string);
    char dash;
    for (auto& part : data_)
    {
        if (iss >> std::hex >> part)
        {
            // Skip the dash characters
            iss >> dash;
        }
    }
}

std::string UGuid::ToString() const
{
    static constexpr const char hexDigits[]{ "0123456789abcdef" };
    std::string result;
    result.reserve(data_.size() * (16llu + 1) - 1); // 16 hex chars per u64 + (N-1) dashes

    for (size i{ 0 }; i < data_.size(); ++i)
    {
        if (i != 0)
        {
            result.push_back('-');
        }

        const u64 value{ data_[i] };
        for (int j{ 60 }; j >= 0; j -= 4)
        {  // 16 nibbles (4 bits per hex digit)
            result.push_back(hexDigits[(value >> j) & 0xF]);
        }
    }

    return result;
}

UGuid::operator std::string() const
{
    return ToString();
}

UGuid& UGuid::operator=(const std::string& string)
{
    std::istringstream iss(string);
    char dash;
    for (auto& part : data_)
    {
        if (iss >> std::hex >> part)
        {
            // Skip the dash characters
            iss >> dash;
        }
    }

    return *this;
}

bool UGuid::operator==(const UGuid& other) const
{
    for (size i{ 0 }; i < data_.size(); i++)
    {
        if (data_[i] != other.data_[i])
        {
            return false;
        }
    }
    return true;
}

size std::hash<UGuid>::operator()(const UGuid& g) const noexcept
{
    ::size h{ 0 };
    combine(h, std::hash<u64>{}(g.data_[0]));
    combine(h, std::hash<u64>{}(g.data_[1]));
    combine(h, std::hash<u64>{}(g.data_[2]));
    combine(h, std::hash<u64>{}(g.data_[3]));
    return h;
}
