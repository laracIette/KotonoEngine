#pragma once
#include "check.h"
#include "types.h"
#include <concepts>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>

/// <summary>
/// A contiguous container with a fixed capacity allocated on the heap
/// </summary>
template <typename T, ::size N>
class UHeapArray final
{
private:
	using ValueType = T;
	using IndexType = ::size;

public:
	UHeapArray()
		: data_{ static_cast<ValueType*>(::operator new[](CapacityBytes(), std::align_val_t{ alignof(ValueType) })) }
		, size_{ 0 }
	{
	}

	template <typename... Args>
		requires (std::constructible_from<ValueType, Args> && ...)
	explicit UHeapArray(Args&&... args)
		: UHeapArray()
	{
		// Fold expression
		(EmplaceBack(std::forward<Args>(args)), ...);
	}

	UHeapArray(UHeapArray&& other) noexcept
		: data_{ std::exchange(other.data_, nullptr) }
		, size_{ std::exchange(other.size_, 0) }
	{
	}

	UHeapArray(UHeapArray const&) = delete;

	~UHeapArray()
	{
		Clear();
		::operator delete[](data_, std::align_val_t{ alignof(ValueType) });
	}

	auto operator=(UHeapArray&& other) noexcept -> UHeapArray&
	{
		if (data_)
		{
			Clear();
			::operator delete[](data_, std::align_val_t{ alignof(ValueType) });
		}

		data_ = std::exchange(other.data_, nullptr);
		size_ = std::exchange(other.size_, 0);
		
		return *this;
	}

	auto operator=(UHeapArray const&) -> UHeapArray& = delete;

	template <typename... Args>
		requires std::constructible_from<ValueType, Args...>
	auto EmplaceBack(Args&&... args) noexcept(std::is_nothrow_constructible_v<ValueType, Args...>) -> ValueType&
	{
		Check(Abort, size_ < Capacity(), "Capacity exceeded!");
		T* const ptr{ std::construct_at(&data_[size_], std::forward<Args>(args)...) };
		++size_;
		return *ptr;
	}

	[[nodiscard]] auto Data() const noexcept -> ValueType*
	{
		return data_;
	}

	[[nodiscard]] auto Size() const noexcept -> IndexType
	{
		return size_;
	}

	[[nodiscard]] auto SizeBytes() const noexcept -> IndexType
	{
		return size_ * sizeof(ValueType);
	}

	[[nodiscard]] static constexpr auto Capacity() noexcept -> IndexType
	{
		return N;
	}

	[[nodiscard]] static constexpr auto CapacityBytes() noexcept -> IndexType
	{
		return N * sizeof(ValueType);
	}

	void Clear() noexcept
	{
		for (IndexType i{ 0 }; i < size_; ++i)
		{
			std::destroy_at(&data_[i]);
		}
		size_ = 0;
	}

	decltype(auto) operator[](this auto&& ctn, IndexType index) noexcept
	{
		Check(Abort, index < ctn.size_, "Index out of bounds!");
		return ctn.data_[index];
	}

	[[nodiscard]] auto begin(this auto&& ctn) noexcept
	{
		return ctn.data_;
	}

	[[nodiscard]] auto end(this auto&& ctn) noexcept
	{
		return ctn.data_ + ctn.size_;
	}

private:
	ValueType* data_;
	IndexType size_;
};
