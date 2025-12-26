#pragma once
#include <cstdint>
enum class EStagingAction : uint8_t
{
	Unregister = 0x01,
	Register = 0x02,
	Refresh = Unregister | Register
};

inline constexpr EStagingAction operator&(EStagingAction first, EStagingAction second)
{
	return static_cast<EStagingAction>(static_cast<uint8_t>(first) & static_cast<uint8_t>(second));
}

inline constexpr EStagingAction operator|(EStagingAction first, EStagingAction second)
{
	return static_cast<EStagingAction>(static_cast<uint8_t>(first) | static_cast<uint8_t>(second));
}
