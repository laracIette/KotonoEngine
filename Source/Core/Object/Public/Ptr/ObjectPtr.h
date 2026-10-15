#pragma once
#include "Ptr.h"

#include "Object/ObjectBase.h"

template <class Base, class Derived>
concept StrictBaseOf = is_strict_base_of_v<Base, Derived>;

template <class TPtr>
class UObjectPtr final : public UPtr<TPtr, KObjectBase>
{
private:
	friend std::hash<UObjectPtr>;
	
	template <class T>
	friend void Delete(UObjectPtr<T> const& ptr);
	
	template <class Derived, StrictBaseOf<Derived> Base>
	friend auto Cast(UObjectPtr<Base>&& ptr) noexcept -> UObjectPtr<Derived>;
	
	template <class Derived, StrictBaseOf<Derived> Base>
	friend auto Cast(UObjectPtr<Base> const& ptr) noexcept -> UObjectPtr<Derived>;
	
	template <class Derived, StrictBaseOf<Derived> Base>
	friend auto TryCast(UObjectPtr<Base>&& ptr) noexcept -> UObjectPtr<Derived>;
	
	template <class Derived, StrictBaseOf<Derived> Base>
	friend auto TryCast(UObjectPtr<Base> const& ptr) noexcept -> UObjectPtr<Derived>;
	
public:
	using UPtr<TPtr, KObjectBase>::UPtr;
};

template <class TPtr>
inline void Delete(UObjectPtr<TPtr> const& ptr)
{
	if (ptr.Data && ptr.Data->pointer)
	{
		delete ptr.Data->pointer;
		ptr.Data->pointer = nullptr;
	}
}

/// Unsafe, equivalent of reinterpret_cast
template <class Derived, StrictBaseOf<Derived> Base>
inline auto Cast(UObjectPtr<Base>&& ptr) noexcept -> UObjectPtr<Derived>
{
	return UObjectPtr<Derived>{ std::move(ptr) };
}

/// Unsafe, equivalent of reinterpret_cast
template <class Derived, StrictBaseOf<Derived> Base>
inline auto Cast(UObjectPtr<Base> const& ptr) noexcept -> UObjectPtr<Derived>
{
	return UObjectPtr<Derived>{ ptr };
}

/// Safe, equivalent of dynamic_cast
template <class Derived, StrictBaseOf<Derived> Base>
inline auto TryCast(UObjectPtr<Base>&& ptr) noexcept -> UObjectPtr<Derived>
{
	if (ptr && dynamic_cast<Derived*>(ptr.Get()))
	{
		return UObjectPtr<Derived>{ std::move(ptr) };
	}
	return nullptr;
}

/// Safe, equivalent of dynamic_cast
template <class Derived, StrictBaseOf<Derived> Base>
inline auto TryCast(UObjectPtr<Base> const& ptr) noexcept -> UObjectPtr<Derived>
{
	if (ptr && dynamic_cast<Derived*>(ptr.Get()))
	{
		return UObjectPtr<Derived>{ ptr };
	}
	return nullptr;
}

template <class TPtr>
struct std::hash<UObjectPtr<TPtr>>
{
	auto operator()(UObjectPtr<TPtr> const& ptr) const noexcept -> ::size
	{
		return std::hash<void*>{}(ptr.Data);
	}
};
