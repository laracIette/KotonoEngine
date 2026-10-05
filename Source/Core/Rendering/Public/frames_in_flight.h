#pragma once
#include <array>
#include <types.h>

inline constexpr size KT_FRAMES_IN_FLIGHT{ 3 };

template <typename T>
using UFramesInFlightArray = std::array<T, KT_FRAMES_IN_FLIGHT>;

template <typename T>
constexpr UFramesInFlightArray<T> make_frames_in_flight_array(T const& fillValue)
{
	UFramesInFlightArray<T> array{};
	array.fill(fillValue);
	return array;
}
