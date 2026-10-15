#pragma once
#include <types.h>
#include <type_traits.h>
#include <utility>

template <class TPtr>
struct UPtrData
{
	TPtr* pointer;
	size count;
};

template <class TPtr, class TData = void>
class UPtr
{	
private:
	template <class, class> 
	friend class UPtr;

	friend std::hash<UPtr>;
	
	template <class TLeftPtr, class TRightPtr, typename Data>
		requires std::is_convertible_v<TRightPtr*, TLeftPtr*>
	friend constexpr auto operator==(UPtr<TLeftPtr, Data> const& left, UPtr<TRightPtr, Data> const& right) noexcept -> b8;

public:
	using PointerType = TPtr;
	using DataType = UPtrData<TData>;

private:
	template <typename Base>
		requires std::is_base_of_v<Base, PointerType>
	explicit constexpr UPtr(UPtr<Base, TData>&& other) noexcept
		: Data{ std::exchange(other.Data, nullptr) }
	{
		TryIncrementCount();
	}
	
	template <typename Base>
		requires std::is_base_of_v<Base, PointerType>
	explicit constexpr UPtr(UPtr<Base, TData> const& other) noexcept
		: Data{ other.Data }
	{
		TryIncrementCount();
	}
	
public:
	/// Creates a new pointer, allocating data
	explicit constexpr UPtr(PointerType* pointer) noexcept
		: Data{ new DataType{ pointer, 1 } }
	{
	}

	constexpr UPtr() noexcept
		: Data{ nullptr }
	{
	}

	constexpr UPtr(std::nullptr_t) noexcept
		: Data{ nullptr }
	{
	}
	
	constexpr UPtr(UPtr&& other) noexcept
		: Data{ std::exchange(other.Data, nullptr) }
	{
	}

	constexpr UPtr(UPtr const& other) noexcept
		: Data{ other.Data }
	{
		TryIncrementCount();
	}

	template <typename From>
		requires std::is_convertible_v<From*, PointerType*>
	constexpr UPtr(UPtr<From, TData>&& other) noexcept
		: Data{ std::exchange(other.Data, nullptr) }
	{
		TryIncrementCount();
	}

	template <typename From>
		requires std::is_convertible_v<From*, PointerType*>
	constexpr UPtr(UPtr<From, TData> const& other) noexcept
		: Data{ other.Data }
	{
		TryIncrementCount();
	}

	constexpr ~UPtr() noexcept
	{
		TryDecrementCount();
	}

	constexpr void Invalidate() const noexcept
	{
		if (Data && Data->pointer)
		{
			Data->pointer = nullptr;
		}
	}

	constexpr auto operator=(UPtr&& other) noexcept -> UPtr&
	{
		TryDecrementCount();

		Data = std::exchange(other.Data, nullptr);

		return *this;
	}

	constexpr auto operator=(UPtr const& other) noexcept -> UPtr&
	{
		if (this == &other)
		{
			return *this;
		}

		TryDecrementCount();

		Data = other.Data;

		TryIncrementCount();
		
		return *this;
	}

	template <typename From>
		requires std::is_convertible_v<From*, PointerType*>
	constexpr auto operator=(UPtr<From, TData>&& other) noexcept -> UPtr&
	{
		TryDecrementCount();

		Data = std::exchange(other.Data, nullptr);

		TryIncrementCount();

		return *this;
	}

	template <typename From>
		requires std::is_convertible_v<From*, PointerType*>
	constexpr auto operator=(UPtr<From, TData> const& other) noexcept -> UPtr&
	{
		TryDecrementCount();

		Data = other.Data;

		TryIncrementCount();

		return *this;
	}

	constexpr auto operator=(std::nullptr_t) noexcept -> UPtr&
	{
		TryDecrementCount();
		Data = nullptr;
		return *this;
	}

	constexpr operator b8() const noexcept
	{
		return Data && Data->pointer;
	}

	constexpr auto Get() const noexcept -> PointerType*
	{
		return Data ? static_cast<PointerType*>(Data->pointer) : nullptr;
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
		if (Data)
		{
			++Data->count;
		}
	}
	
	constexpr void TryDecrementCount() const noexcept
	{
		if (Data && --Data->count == 0)
		{
			delete Data;
		}
	}

protected:
	DataType* Data;
};

template <class TLeftPtr, class TRightPtr, typename TData>
	requires std::is_convertible_v<TRightPtr*, TLeftPtr*>
constexpr auto operator==(UPtr<TLeftPtr, TData> const& left, UPtr<TRightPtr, TData> const& right) noexcept -> b8
{
	return left.Data == right.Data;
}

template <class TLeftPtr, class TRightPtr, typename TLeftData, typename TRightData>
constexpr auto operator==(UPtr<TLeftPtr, TLeftData>, UPtr<TRightPtr, TRightData>) noexcept -> b8 = delete;

template <class TPtr, typename TData>
constexpr auto operator==(UPtr<TPtr, TData> ptr, std::nullptr_t) noexcept -> b8
{
	return !ptr;
}

template <class TPtr, class TData>
struct std::hash<UPtr<TPtr, TData>>
{
	auto operator()(UPtr<TPtr, TData> const& ptr) const noexcept -> ::size
	{
		return std::hash<void*>{}(ptr.Data);
	}
};
