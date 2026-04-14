#pragma once
#include "types.h"
#include <concepts>
#include <utility>

enum class EPropertyType : u8
{
	Writable,
	Readonly
};

template <EPropertyType PropertyType>
concept Writable = (PropertyType == EPropertyType::Writable);

template <EPropertyType PropertyType, typename T>
class UProperty final
{
public:
	using ValueType = T;

public:
	constexpr UProperty() noexcept : value_() {}
	constexpr UProperty(const ValueType& value) noexcept : value_(value) {}
	constexpr UProperty(ValueType&& value) noexcept : value_(std::move(value)) {}

	constexpr const ValueType& Get() const noexcept
	{
		return value_;
	}

	constexpr operator ValueType() const noexcept
	{
		return Get();
	}

	constexpr void Set(const ValueType& value) noexcept
		requires Writable<PropertyType>
	{
		value_ = value;
	}

	constexpr UProperty& operator=(const UProperty& property) noexcept
		requires Writable<PropertyType>
	{
		value_ = property.value_;
		return *this;
	}

	constexpr UProperty& operator=(const ValueType& value) noexcept
		requires Writable<PropertyType>
	{
		value_ = value;
		return *this;
	}

	constexpr UProperty& operator=(ValueType&& value) noexcept
		requires Writable<PropertyType>
	{
		value_ = std::move(value);
		return *this;
	}

private:
	ValueType value_;
};

template <typename T>
using ReadonlyProperty = UProperty<EPropertyType::Readonly, T>;
template <typename T>
using WritableProperty = UProperty<EPropertyType::Writable, T>;
