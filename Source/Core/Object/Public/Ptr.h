#pragma once
#include <format>
#include <types.h>
#include <string>
#include <type_traits>
#include <utility>

struct UPtrData
{
	void* pointer;
	size count;
};

template <class T>
class UPtr final
{
private:
	template <typename U> 
	friend class UPtr;

	friend std::hash<UPtr>;
	
	template <typename Derived, typename Base>
		requires std::is_base_of_v<Base, Derived>
	friend auto Cast(UPtr<Base> const& ptr) noexcept -> UPtr<Derived>;
	
	template <typename Derived, typename Base>
		requires std::is_base_of_v<Base, Derived>
	friend auto Cast(UPtr<Base>&& ptr) noexcept -> UPtr<Derived>;
	
	template <typename Derived, typename Base>
		requires std::is_base_of_v<Base, Derived>
	friend auto TryCast(UPtr<Base> const& ptr) noexcept -> UPtr<Derived>;
	
	template <typename Derived, typename Base>
		requires std::is_base_of_v<Base, Derived>
	friend auto TryCast(UPtr<Base>&& ptr) noexcept -> UPtr<Derived>;

public:
	using PointerType = T;
	using Data = UPtrData;

private:
	template <typename Base>
		requires std::is_base_of_v<Base, PointerType>
	constexpr UPtr(UPtr<Base>&& other) noexcept
		: data_{ std::exchange(other.data_, nullptr) }
	{
		TryIncrementCount();
	}
	
	template <typename Base>
		requires std::is_base_of_v<Base, PointerType>
	constexpr UPtr(UPtr<Base> const& other) noexcept
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

	explicit constexpr UPtr(PointerType* pointer) noexcept
		: data_{ new Data{ pointer, 1 } }
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
		if (this == &other)
		{
			return *this;
		}

		TryDecrementCount();

		data_ = std::exchange(other.data_, nullptr);

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

	constexpr auto operator==(UPtr const& other) const noexcept -> b8
	{
		return data_ == other.data_;
	}

	template <typename From>
		requires std::is_convertible_v<From*, PointerType*>
	constexpr auto operator==(UPtr<From> const& other) const noexcept -> b8
	{
		return data_ == other.data_;
	}

	constexpr auto operator==(std::nullptr_t) const noexcept -> b8
	{
		return !operator b8();
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

	operator std::string() const
	{
		return Get() ? Get()->operator std::string() : std::string{ "nullptr" };
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
	Data* data_;
};

/// Unsafe, equivalent of reinterpret_cast
template <typename Derived, typename Base>
	requires std::is_base_of_v<Base, Derived>
inline auto Cast(UPtr<Base> const& ptr) noexcept -> UPtr<Derived>
{
	return UPtr<Derived>{ ptr };
}

/// Unsafe, equivalent of reinterpret_cast
template <typename Derived, typename Base>
	requires std::is_base_of_v<Base, Derived>
inline auto Cast(UPtr<Base>&& ptr) noexcept -> UPtr<Derived>
{
	return UPtr<Derived>{ std::move(ptr) };
}

/// Safe, equivalent of dynamic_cast
template <typename Derived, typename Base>
	requires std::is_base_of_v<Base, Derived>
inline auto TryCast(UPtr<Base> const& ptr) noexcept -> UPtr<Derived>
{
	if (ptr && dynamic_cast<Derived*>(ptr.Get()))
	{
		return UPtr<Derived>{ ptr };
	}
	return nullptr;
}

/// Safe, equivalent of dynamic_cast
template <typename Derived, typename Base>
	requires std::is_base_of_v<Base, Derived>
inline auto TryCast(UPtr<Base>&& ptr) noexcept -> UPtr<Derived>
{
	if (ptr && dynamic_cast<Derived*>(ptr.Get()))
	{
		return UPtr<Derived>{ std::move(ptr) };
	}
	return nullptr;
}

template <typename T>
struct std::hash<UPtr<T>>
{
	auto operator()(UPtr<T> const& ptr) const noexcept -> ::size
	{
		return std::hash<void*>{}(ptr.data_);
	}
};

template <typename T, typename CharT>
struct std::formatter<UPtr<T>, CharT> : std::formatter<std::string, CharT>
{
    auto format(UPtr<T> const& ptr, auto& ctx) const 
	{
        return std::format_to(ctx.out(), "{0}", ptr.operator std::string());
    }
};
