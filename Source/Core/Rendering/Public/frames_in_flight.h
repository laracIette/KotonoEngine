#pragma once

#include "Containers/Array.h"
#include "types.h"

inline constexpr ::size KT_FRAMES_IN_FLIGHT{ 3 };

template <typename T>
using UFramesInFlightArray = UArray<T, KT_FRAMES_IN_FLIGHT>;

template <typename T>
constexpr UFramesInFlightArray<T> make_frames_in_flight_array(T const& fillValue)
{
	UFramesInFlightArray<T> array{};
	array.Fill(fillValue);
	return array;
}
