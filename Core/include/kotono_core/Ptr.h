#pragma once
#include <format>
#include <kotono_common/types.h>
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

public:
	using PointerType = T;
	using Data = UPtrData;

public:
	constexpr UPtr() noexcept
		: data_{ nullptr }
	{
	}

	constexpr UPtr(std::nullptr_t) noexcept
		: data_{ nullptr }
	{
	}

	explicit constexpr UPtr(void* pointer) noexcept
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
		if (data_)
		{
			++data_->count;
		}
	}

	template <typename From>
		requires std::is_convertible_v<From*, PointerType*>
	constexpr UPtr(UPtr<From> const& other) noexcept
		: data_{ other.data_ }
	{
		if (data_)
		{
			++data_->count;
		}
	}

	// Equivalent of static_cast
	template <typename Base>
		requires std::is_base_of_v<Base, PointerType>
	constexpr UPtr(UPtr<Base> const& other) noexcept
		: data_{ other.data_ }
	{
		if (data_)
		{
			++data_->count;
		}
	}

	constexpr ~UPtr() noexcept
	{
		if (data_ && --data_->count == 0)
		{
			delete data_;
		}
	}

	constexpr void Invalidate() noexcept
	{
		if (data_)
		{
			data_->pointer = nullptr;
		}
	}

	constexpr UPtr& operator=(UPtr&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		if (data_ && --data_->count == 0)
		{
			delete data_;
		}

		data_ = std::exchange(other.data_, nullptr);

		return *this;
	}

	constexpr UPtr& operator=(std::nullptr_t) noexcept
	{
		if (data_ && --data_->count == 0)
		{
			delete data_;
		}
		data_ = nullptr;
		return *this;
	}

	template <typename From>
		requires std::is_convertible_v<From*, PointerType*>
	constexpr UPtr& operator=(UPtr<From> const& other) noexcept
	{
		if (data_ && --data_->count == 0)
		{
			delete data_;
		}

		data_ = other.data_;

		if (data_)
		{
			++data_->count;
		}

		return *this;
	}

	constexpr UPtr& operator=(UPtr const& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		if (data_ && --data_->count == 0)
		{
			delete data_;
		}

		data_ = other.data_;

		if (data_)
		{
			++data_->count;
		}
		
		return *this;
	}

	constexpr b8 operator==(UPtr const& other) const noexcept
	{
		return data_ == other.data_;
	}

	template <typename From>
		requires std::is_convertible_v<From*, PointerType*>
	constexpr b8 operator==(UPtr<From> const& other) const noexcept
	{
		return data_ == other.data_;
	}

	constexpr b8 operator==(std::nullptr_t) const noexcept
	{
		return !operator b8();
	}

	constexpr operator b8() const noexcept
	{
		return data_ && data_->pointer;
	}

	constexpr PointerType* Get() const noexcept
	{
		return data_ ? static_cast<PointerType*>(data_->pointer) : nullptr;
	}

	constexpr PointerType* operator->() const noexcept
	{
		return Get();
	}

	constexpr PointerType& operator*() const noexcept
	{
		return *Get();
	}

	operator std::string() const
	{
		return Get() ? Get()->operator std::string() : std::string{ "nullptr" };
	}
	
private:
	Data* data_;
};

// Equivalent of dynamic_cast
template <typename Derived, typename Base>
	requires std::is_base_of_v<Base, Derived>
inline UPtr<Derived> TryCast(UPtr<Base> const& ptr)
{
	if (ptr && dynamic_cast<Derived*>(ptr.Get()))
	{
		return UPtr<Derived>{ ptr };
	}
	return nullptr;
}

template <typename T>
struct std::hash<UPtr<T>>
{
	::size operator()(UPtr<T> const& ptr) const noexcept
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
