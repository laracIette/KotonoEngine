#include "Guid.h"
#include <iostream>
#include <sstream>
#include <iomanip>

UGuid::UGuid()
{
	if (CoCreateGuid(&_guid) != S_OK)
	{
		std::cerr << "Failed to create GUID!" << std::endl;
	}
}

UGuid::operator std::string() const
{
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');

    oss << std::setw(8) << _guid.Data1
        << std::setw(4) << _guid.Data2
        << std::setw(4) << _guid.Data3;

    for (auto byte : _guid.Data4)
        oss << std::setw(2) << static_cast<int>(byte);

    return oss.str();
}

void UGuid::operator=(const std::string& string)
{
    std::istringstream iss(string);

    iss >> std::hex >> _guid.Data1
        >> _guid.Data2
        >> _guid.Data3;

    for (int i = 0; i < 8; ++i)
    {
        int byte;
        iss >> std::hex >> byte;
        _guid.Data4[i] = static_cast<uint8_t>(byte);
    }
}

bool UGuid::operator==(const UGuid& other) const
{
	return _guid == other._guid;
}
