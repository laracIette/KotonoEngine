#pragma once
#include <charconv>
#include <string_view>

template<typename T>
static T from_string(std::string_view str, T defaultValue = T{})
{
    T result;
    auto [ptr, ec] { std::from_chars(str.data(), str.data() + str.size(), result) };
    return ec == std::errc{} ? result : defaultValue;
}
