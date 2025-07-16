#pragma once
#include <array>
constexpr size_t KT_FRAMES_IN_FLIGHT = 3;
template <typename T>
using KtFramesInFlightArray = std::array<T, KT_FRAMES_IN_FLIGHT>;