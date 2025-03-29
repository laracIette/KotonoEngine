#include "Guid.h"
#include <random>
#include <sstream>
#include <iomanip>

UGuid::UGuid()
{
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dist;

    for (auto& part : _data)
    {
        part = dist(gen);
    }

    // Set the version bits (4 bits for GUID v4)
    _data[0] &= 0xFFFFFFFFFFFFFF0F; // Clear version bits
    _data[0] |= 0x0000000000000040; // Set version 4 (random GUID)
}

UGuid::operator std::string() const
{
    std::ostringstream oss;
    for (size_t i = 0; i < _data.size(); i++)
    {
        if (i != 0)
        {
            oss << '-';
        }
        oss << std::hex << std::setw(16) << std::setfill('0') << _data[i];
    }
    return oss.str();
}

void UGuid::operator=(const std::string& string)
{
    std::istringstream iss(string);
    char dash;
    for (auto& part : _data)
    {
        if (iss >> std::hex >> part)
        {
            // Skip the dash characters
            iss >> dash;
        }
    }
}

bool UGuid::operator==(const UGuid& other) const
{
	for (size_t i = 0; i < _data.size(); i++)
	{
		if (_data[i] != other._data[i])
		{
			return false;
		}
	}
	return true;
}
