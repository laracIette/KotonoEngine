#pragma once
#include "types.h"
#include <cassert>
#include <concepts>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>
template <typename T, ::size Capacity>
/// <summary>
/// An contiguous container with a fixed capacity allocated on the heap
/// </summary>
class UFixedHeapContainer final
{
private:
	using ValueType = T;
	using IndexType = ::size;

public:
	using value_type		= ValueType;
	using index_type		= IndexType;
	using iterator			= ValueType*;
	using const_iterator	= ValueType const*;

public:
	UFixedHeapContainer()
		: data_{ static_cast<ValueType*>(::operator new[](capacity_bytes(), std::align_val_t{ alignof(ValueType) })) }
		, size_{ 0 }
	{
	}

	template <typename... Args>
		requires (std::constructible_from<ValueType, Args> && ...)
	UFixedHeapContainer(Args&&... args)
		: UFixedHeapContainer()
	{
		// Fold expression
		(emplace_back(std::forward<Args>(args)), ...);
	}

	UFixedHeapContainer(UFixedHeapContainer&& other) noexcept
		: data_{ std::exchange(other.data_, nullptr) }
		, size_{ std::exchange(other.size_, 0) }
	{
	}

	UFixedHeapContainer(UFixedHeapContainer const&) = delete;
	UFixedHeapContainer& operator=(UFixedHeapContainer const&) = delete;

	~UFixedHeapContainer()
	{
		clear();
		::operator delete[](data_, std::align_val_t{ alignof(ValueType) });
	}

	UFixedHeapContainer& operator=(UFixedHeapContainer&& other) noexcept
	{
		if (this != &other)
		{
			if (data_)
			{
				clear();
				::operator delete[](data_, std::align_val_t{ alignof(ValueType) });
			}
			data_ = std::exchange(other.data_, nullptr);
			size_ = std::exchange(other.size_, 0);
		}
		return *this;
	}

	template <typename... Args>
		requires std::constructible_from<ValueType, Args...>
	ValueType& emplace_back(Args&&... args) noexcept(std::is_nothrow_constructible_v<ValueType, Args...>)
	{
		assert(size_ < Capacity && "UFixedHeapContainer capacity exceeded!");
		T* const ptr{ std::construct_at(&data_[size_], std::forward<Args>(args)...) };
		++size_;
		return *ptr;
	}

	ValueType* data() const noexcept
	{
		return data_;
	}

	IndexType size() const noexcept
	{
		return size_;
	}

	IndexType size_bytes() const noexcept
	{
		return size_ * sizeof(ValueType);
	}

	static constexpr IndexType capacity() noexcept
	{
		return Capacity;
	}

	static constexpr IndexType capacity_bytes() noexcept
	{
		return Capacity * sizeof(ValueType);
	}

	void clear() noexcept
	{
		for (IndexType i{ 0 }; i < size_; ++i)
		{
			std::destroy_at(&data_[i]);
		}
		size_ = 0;
	}

	auto begin(this auto&& ctn) noexcept
	{
		return ctn.data_;
	}

	auto end(this auto&& ctn) noexcept
	{
		return ctn.data_ + ctn.size_;
	}

	decltype(auto) operator[](this auto&& ctn, IndexType index) noexcept
	{
		assert(index < size_ && "Index out of bounds!");
		return ctn.data_[index];
	}

private:
	ValueType* data_;
	IndexType size_;
};
