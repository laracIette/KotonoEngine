#pragma once
#include <array>
#include <kotono_common/types.h>
inline constexpr size KT_FRAMES_IN_FLIGHT{ 3 };
template <typename T>
using UFramesInFlightArray = std::array<T, KT_FRAMES_IN_FLIGHT>;

template <typename T>
static constexpr UFramesInFlightArray<T> make_frames_in_flight_array(const T& fillValue)
{
	UFramesInFlightArray<T> array{};
	array.fill(fillValue);
	return array;
}
