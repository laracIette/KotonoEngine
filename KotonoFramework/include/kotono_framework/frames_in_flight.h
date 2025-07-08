#pragma once
#include <array>
constexpr size_t KT_FRAMES_IN_FLIGHT = 3;
template <typename T>
using FramesInFlightArray = std::array<T, KT_FRAMES_IN_FLIGHT>;