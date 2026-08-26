#pragma once
#include <kotono_common/types.h>
#include <nlohmann/json_fwd.hpp>
#include <ranges>
#include <type_traits>

#define SERIALIZE

class UPath;

b8 contains(nlohmann::json const& json, std::string_view name);

nlohmann::json& get(nlohmann::json& json, std::string_view name);
nlohmann::json const& get(nlohmann::json const& json, std::string_view name);

void make_array(nlohmann::json& json);
nlohmann::json& get_next(nlohmann::json& json);

size get_size(nlohmann::json const& json);
nlohmann::json const& get_at(nlohmann::json const& json, size index);

template<typename T>
struct USerialize
{
	void operator()(nlohmann::json& json, T const& v) const;
};

template<>
void USerialize<b8>::operator()(nlohmann::json& json, b8 const& v) const;

template<>
void USerialize<i8>::operator()(nlohmann::json& json, i8 const& v) const;

template<>
void USerialize<i16>::operator()(nlohmann::json& json, i16 const& v) const;

template<>
void USerialize<i32>::operator()(nlohmann::json& json, i32 const& v) const;

template<>
void USerialize<i64>::operator()(nlohmann::json& json, i64 const& v) const;

template<>
void USerialize<u8>::operator()(nlohmann::json& json, u8 const& v) const;

template<>
void USerialize<u16>::operator()(nlohmann::json& json, u16 const& v) const;

template<>
void USerialize<u32>::operator()(nlohmann::json& json, u32 const& v) const;

template<>
void USerialize<u64>::operator()(nlohmann::json& json, u64 const& v) const;

template<>
void USerialize<f32>::operator()(nlohmann::json& json, f32 const& v) const;

template<>
void USerialize<f64>::operator()(nlohmann::json& json, f64 const& v) const;

template<>
void USerialize<std::string>::operator()(nlohmann::json& json, std::string const& v) const;

template<>
void USerialize<UPath>::operator()(nlohmann::json& json, UPath const& v) const;

template<typename T>
	requires std::is_enum_v<T>
struct USerialize<T>
{
	void operator()(nlohmann::json& json, const T v) const
	{
		using enum_t = std::underlying_type_t<T>;
		USerialize<enum_t>{}(json, static_cast<enum_t>(v));
	}
};

template<typename T>
	requires std::ranges::range<T> && (!std::convertible_to<T, std::string>)
struct USerialize<T>
{
	void operator()(nlohmann::json& json, T const& v) const
	{
		using item_t = std::ranges::range_value_t<T>;
		make_array(json);
		for (item_t const& item : v)
		{
			USerialize<item_t>{}(get_next(json), item);
		}
	}
};

template<typename T>
struct UDeserialize
{
	void operator()(nlohmann::json const& json, T& v) const
	{
		if constexpr (std::is_enum_v<T>)
		{
			using enum_t = std::underlying_type_t<T>;
			v = static_cast<T>(UDeserialize<enum_t>{}(json));
		}
		else if constexpr (std::ranges::range<T>
			&& requires(T& container, std::ranges::range_value_t<T> item) { container.push_back(item); }
			&& !std::convertible_to<T, std::string>
		)
		{
			using item_t = std::ranges::range_value_t<T>;

			size const rangeSize{ get_size(json) };

			if constexpr (requires { v.reserve(rangeSize); })
			{
				v.reserve(rangeSize);
			}

			for (size i{ 0 }; i < rangeSize; ++i)
			{
				item_t value{};
				UDeserialize<item_t>{}(get_at(json, i), value);
				v.push_back(std::move(value));
			}
		}
	}

	T operator()(nlohmann::json const& json) const
	{
		T v{};
		operator()(json, v);
		return v;
	}
};

template<>
void UDeserialize<b8>::operator()(nlohmann::json const& json, b8& v) const;

template<>
void UDeserialize<i8>::operator()(nlohmann::json const& json, i8& v) const;

template<>
void UDeserialize<i16>::operator()(nlohmann::json const& json, i16& v) const;

template<>
void UDeserialize<i32>::operator()(nlohmann::json const& json, i32& v) const;

template<>
void UDeserialize<i64>::operator()(nlohmann::json const& json, i64& v) const;

template<>
void UDeserialize<u8>::operator()(nlohmann::json const& json, u8& v) const;

template<>
void UDeserialize<u16>::operator()(nlohmann::json const& json, u16& v) const;

template<>
void UDeserialize<u32>::operator()(nlohmann::json const& json, u32& v) const;

template<>
void UDeserialize<u64>::operator()(nlohmann::json const& json, u64& v) const;

template<>
void UDeserialize<f32>::operator()(nlohmann::json const& json, f32& v) const;

template<>
void UDeserialize<f64>::operator()(nlohmann::json const& json, f64& v) const;

template<>
void UDeserialize<std::string>::operator()(nlohmann::json const& json, std::string& v) const;

template<>
void UDeserialize<UPath>::operator()(nlohmann::json const& json, UPath& v) const;
