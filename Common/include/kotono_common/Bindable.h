#pragma once
#include <concepts>
#include <functional>
#include <kotono_common/types.h>
#include <variant>
template <typename T>
class UBindable final
{
private:
	using ValueType = T;
	using FuncType = std::function<T()>;
	using VariantType = std::variant<ValueType, FuncType>;

public:
	UBindable() 
		: value_{ ValueType{} } 
	{}

	template <typename TArg>
		requires std::constructible_from<ValueType, TArg>
	UBindable(TArg const& value) 
		: value_{ ValueType{ value } }
	{}

	template <typename TArg>
		requires std::constructible_from<FuncType, TArg>
	UBindable(TArg const& func)
		: value_{ FuncType{ func } }
	{}

	constexpr ValueType Get() const
	{
		return GetIsValue()
			? std::get<ValueType>(value_)
			: std::get<FuncType>(value_)();
	}

	constexpr operator ValueType() const
	{
		return Get();
	}

	constexpr b8 GetIsValue() const noexcept
	{
		return std::holds_alternative<ValueType>(value_);
	}

	constexpr b8 GetIsFunction() const noexcept
	{
		return std::holds_alternative<FuncType>(value_);
	}

private:
	VariantType value_;
};