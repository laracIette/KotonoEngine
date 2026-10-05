#pragma once
#include "types.h"
#include <array>

template <typename T, size Cols, size Rows>
class UMatrix
{
public:
	constexpr decltype(auto) operator[](this auto&& self, size col, size row) noexcept
	{
		return self.data_[col][row];
	}

private:
	std::array<std::array<T, Rows>, Cols> data_;
};