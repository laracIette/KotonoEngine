#pragma once
#include <concepts>
#include <functional>
#include "types.h"
#include <variant>
template <typename T>
class UBindable final
{
private:
	using ValueType = T;
	using FuncType = std::function<T()>;
	using VariantType = std::variant<ValueType, FuncType>;

public:
	UBindable() = default;

	template <typename TArg>
		requires std::constructible_from<ValueType, TArg>
			&& !std::same_as<std::remove_cvref_t<TArg>, UBindable>
	UBindable(TArg&& value) 
		: value_{ std::in_place_type<ValueType>, std::forward<TArg>(value) }
	{
	}

	template <typename TArg>
		requires std::constructible_from<FuncType, TArg>
			&& !std::same_as<std::remove_cvref_t<TArg>, UBindable>
	UBindable(TArg&& func)
		: value_{ std::in_place_type<FuncType>, std::forward<TArg>(func) }
	{
	}

	constexpr auto Get() const -> ValueType
	{
		return GetIsValue()
			? std::get<ValueType>(value_)
			: std::get<FuncType>(value_)();
	}

	constexpr operator ValueType() const
	{
		return Get();
	}

	constexpr auto GetIsValue() const noexcept -> b8
	{
		return std::holds_alternative<ValueType>(value_);
	}

	constexpr auto GetIsFunction() const noexcept -> b8
	{
		return std::holds_alternative<FuncType>(value_);
	}

private:
	VariantType value_;
};