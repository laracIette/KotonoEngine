#pragma once

#include "Containers/Array.h"
#include "types.h"

template <typename T, size Cols, size Rows>
class UMatrix final
{
public:
	constexpr decltype(auto) operator[](this auto&& self, size col, size row) noexcept
	{
		return self.data_[col][row];
	}

private:
	UArray<UArray<T, Rows>, Cols> data_;
};