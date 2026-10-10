#pragma once
#include <types.h>
#include <type_traits>
#include <utility>

template <class TPtr>
struct UPtrData
{
	TPtr* pointer;
	size count;
};

template <class TPtr, class TData = UPtrData<void>>
class UPtr final
{	
private:
	template <class, class> 
	friend class UPtr;

	friend std::hash<UPtr>;
	
	template <class Derived, class Base, typename Data>
		requires std::is_base_of_v<Base, Derived>
	friend auto Cast(UPtr<Base, Data>&& ptr) noexcept -> UPtr<Derived, Data>;
	
	template <class Derived, class Base, typename Data>
		requires std::is_base_of_v<Base, Derived>
	friend auto Cast(UPtr<Base, Data> const& ptr) noexcept -> UPtr<Derived, Data>;
	
	template <class Derived, class Base, typename Data>
		requires std::is_base_of_v<Base, Derived>
	friend auto TryCast(UPtr<Base, Data>&& ptr) noexcept -> UPtr<Derived, Data>;
	
	template <class Derived, class Base, typename Data>
		requires std::is_base_of_v<Base, Derived>
	friend auto TryCast(UPtr<Base, Data> const& ptr) noexcept -> UPtr<Derived, Data>;

public:
	using PointerType = TPtr;
	using DataType = TData;

private:
	template <typename Base>
		requires std::is_base_of_v<Base, PointerType>
	explicit constexpr UPtr(UPtr<Base>&& other) noexcept
		: data_{ std::exchange(other.data_, nullptr) }
	{
		TryIncrementCount();
	}
	
	template <typename Base>
		requires std::is_base_of_v<Base, PointerType>
	explicit constexpr UPtr(UPtr<Base> const& other) noexcept
		: data_{ other.data_ }
	{
		TryIncrementCount();
	}
	
public:
	constexpr UPtr() noexcept
		: data_{ nullptr }
	{
	}

	constexpr UPtr(std::nullptr_t) noexcept
		: data_{ nullptr }
	{
	}

	/// Creates a new pointer, allocating data
	explicit constexpr UPtr(PointerType* pointer) noexcept
		: data_{ new DataType{ pointer, 1 } }
	{
	}
	
	constexpr UPtr(UPtr&& other) noexcept
		: data_{ std::exchange(other.data_, nullptr) }
	{
	}

	constexpr UPtr(UPtr const& other) noexcept
		: data_{ other.data_ }
	{
		TryIncrementCount();
	}

	template <typename From>
		requires std::is_convertible_v<From*, PointerType*>
	constexpr UPtr(UPtr<From>&& other) noexcept
		: data_{ std::exchange(other.data_, nullptr) }
	{
		TryIncrementCount();
	}

	template <typename From>
		requires std::is_convertible_v<From*, PointerType*>
	constexpr UPtr(UPtr<From> const& other) noexcept
		: data_{ other.data_ }
	{
		TryIncrementCount();
	}

	constexpr ~UPtr() noexcept
	{
		TryDecrementCount();
	}

	constexpr void Invalidate() const noexcept
	{
		if (data_)
		{
			data_->pointer = nullptr;
		}
	}

	constexpr auto operator=(UPtr&& other) noexcept -> UPtr&
	{
		TryDecrementCount();

		data_ = std::exchange(other.data_, nullptr);

		return *this;
	}

	constexpr auto operator=(UPtr const& other) noexcept -> UPtr&
	{
		if (this == &other)
		{
			return *this;
		}

		TryDecrementCount();

		data_ = other.data_;

		TryIncrementCount();
		
		return *this;
	}

	constexpr auto operator=(std::nullptr_t) noexcept -> UPtr&
	{
		TryDecrementCount();
		data_ = nullptr;
		return *this;
	}

	template <typename From>
		requires std::is_convertible_v<From*, PointerType*>
	constexpr auto operator=(UPtr<From> const& other) noexcept -> UPtr&
	{
		TryDecrementCount();

		data_ = other.data_;

		TryIncrementCount();

		return *this;
	}

	constexpr operator b8() const noexcept
	{
		return data_ && data_->pointer;
	}

	constexpr auto Get() const noexcept -> PointerType*
	{
		return data_ ? static_cast<PointerType*>(data_->pointer) : nullptr;
	}

	constexpr auto operator->() const noexcept -> PointerType*
	{
		return Get();
	}

	constexpr auto operator*() const noexcept -> PointerType&
	{
		return *Get();
	}
	
private:
	constexpr void TryIncrementCount() const noexcept
	{
		if (data_)
		{
			++data_->count;
		}
	}
	
	constexpr void TryDecrementCount() const noexcept
	{
		if (data_ && --data_->count == 0)
		{
			delete data_;
		}
	}

private:
	DataType* data_;
};

template <class TLeftPtr, class TRightPtr, typename TData>
	requires std::is_convertible_v<TRightPtr*, TLeftPtr*>
constexpr auto operator==(UPtr<TLeftPtr, TData> const& left, UPtr<TRightPtr, TData> const& right) noexcept -> b8
{
	return left.Get() == right.Get();
}

template <class TLeftPtr, class TRightPtr, typename TLeftData, typename TRightData>
constexpr auto operator==(UPtr<TLeftPtr, TLeftData>, UPtr<TRightPtr, TRightData>) noexcept -> b8 = delete;

template <class TPtr, typename TData>
constexpr auto operator==(UPtr<TPtr, TData> ptr, std::nullptr_t) noexcept -> b8
{
	return !ptr;
}

/// Unsafe, equivalent of reinterpret_cast
template <class Derived, class Base, typename TData>
	requires std::is_base_of_v<Base, Derived>
inline auto Cast(UPtr<Base, TData>&& ptr) noexcept -> UPtr<Derived, TData>
{
	return UPtr<Derived>{ std::move(ptr) };
}

/// Unsafe, equivalent of reinterpret_cast
template <class Derived, class Base, typename TData>
	requires std::is_base_of_v<Base, Derived>
inline auto Cast(UPtr<Base, TData> const& ptr) noexcept -> UPtr<Derived, TData>
{
	return UPtr<Derived, TData>{ ptr };
}

/// Safe, equivalent of dynamic_cast
template <class Derived, class Base, typename TData>
	requires std::is_base_of_v<Base, Derived>
inline auto TryCast(UPtr<Base, TData>&& ptr) noexcept -> UPtr<Derived, TData>
{
	if (ptr && dynamic_cast<Derived*>(ptr.Get()))
	{
		return UPtr<Derived, TData>{ std::move(ptr) };
	}
	return nullptr;
}

/// Safe, equivalent of dynamic_cast
template <class Derived, class Base, typename TData>
	requires std::is_base_of_v<Base, Derived>
inline auto TryCast(UPtr<Base, TData> const& ptr) noexcept -> UPtr<Derived, TData>
{
	if (ptr && dynamic_cast<Derived*>(ptr.Get()))
	{
		return UPtr<Derived, TData>{ ptr };
	}
	return nullptr;
}

template <class TPtr, class TData>
struct std::hash<UPtr<TPtr, TData>>
{
	auto operator()(UPtr<TPtr, TData> const& ptr) const noexcept -> ::size
	{
		return std::hash<void*>{}(ptr.data_);
	}
};
