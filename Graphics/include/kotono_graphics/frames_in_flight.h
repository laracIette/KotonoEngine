#pragma once
#include <array>
#include <kotono_common/types.h>
inline constexpr size KT_FRAMES_IN_FLIGHT{ 3 };
template <typename T>
using KtFramesInFlightArray = std::array<T, KT_FRAMES_IN_FLIGHT>;