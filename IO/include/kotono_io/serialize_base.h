#pragma once
#include <kotono_common/Asset.h>
#include <kotono_common/AssetManager.h>
#include <kotono_common/types.h>
#include <nlohmann/json_fwd.hpp>
#include <ranges>
#include <type_traits>

#define SERIALIZE

class UPath;

bool contains(const nlohmann::json& json, const std::string_view name);

nlohmann::json& get(nlohmann::json& json, const std::string_view name);
const nlohmann::json& get(const nlohmann::json& json, const std::string_view name);

void make_array(nlohmann::json& json);
nlohmann::json& get_next(nlohmann::json& json);

size get_size(const nlohmann::json& json);
const nlohmann::json& get_at(const nlohmann::json& json, size index);

template<typename T>
struct USerialize
{
};

template<>
struct USerialize<bool>
{
	void operator()(nlohmann::json& json, const bool v) const;
};

template<>
struct USerialize<i8>
{
	void operator()(nlohmann::json& json, const i8 v) const;
};

template<>
struct USerialize<i16>
{
	void operator()(nlohmann::json& json, const i16 v) const;
};

template<>
struct USerialize<i32>
{
	void operator()(nlohmann::json& json, const i32 v) const;
};

template<>
struct USerialize<i64>
{
	void operator()(nlohmann::json& json, const i64 v) const;
};

template<>
struct USerialize<u8>
{
	void operator()(nlohmann::json& json, const u8 v) const;
};

template<>
struct USerialize<u16>
{
	void operator()(nlohmann::json& json, const u16 v) const;
};

template<>
struct USerialize<u32>
{
	void operator()(nlohmann::json& json, const u32 v) const;
};

template<>
struct USerialize<u64>
{
	void operator()(nlohmann::json& json, const u64 v) const;
};

template<>
struct USerialize<f32>
{
	void operator()(nlohmann::json& json, const f32 v) const;
};

template<>
struct USerialize<f64>
{
	void operator()(nlohmann::json& json, const f64 v) const;
};

template<>
struct USerialize<std::string>
{
	void operator()(nlohmann::json& json, const std::string& v) const;
};

template<>
struct USerialize<UPath>
{
	void operator()(nlohmann::json& json, const UPath& v) const;
};

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
	void operator()(nlohmann::json& json, const T& v) const
	{
		using item_t = std::ranges::range_value_t<T>;
		make_array(json);
		for (const item_t& item : v)
		{
			USerialize<item_t>{}(get_next(json), item);
		}
	}
};

template<std::derived_from<AAsset> T>
struct USerialize<T>
{
	void operator()(nlohmann::json& json, T const* v) const
	{
		if (v)
		{
			USerialize<UPath>{}(json, v->GetPath());
		}
	}
};

template<typename T>
struct UDeserialize
{};

template<>
struct UDeserialize<bool>
{
	void operator()(const nlohmann::json& json, bool& v) const;
};

template<>
struct UDeserialize<i8>
{
	void operator()(const nlohmann::json& json, i8& v) const;
};

template<>
struct UDeserialize<i16>
{
	void operator()(const nlohmann::json& json, i16& v) const;
};

template<>
struct UDeserialize<i32>
{
	void operator()(const nlohmann::json& json, i32& v) const;
};

template<>
struct UDeserialize<i64>
{
	void operator()(const nlohmann::json& json, i64& v) const;
};

template<>
struct UDeserialize<u8>
{
	void operator()(const nlohmann::json& json, u8& v) const;
};

template<>
struct UDeserialize<u16>
{
	void operator()(const nlohmann::json& json, u16& v) const;
};

template<>
struct UDeserialize<u32>
{
	void operator()(const nlohmann::json& json, u32& v) const;
};

template<>
struct UDeserialize<u64>
{
	void operator()(const nlohmann::json& json, u64& v) const;
};

template<>
struct UDeserialize<f32>
{
	void operator()(const nlohmann::json& json, f32& v) const;
};

template<>
struct UDeserialize<f64>
{
	void operator()(const nlohmann::json& json, f64& v) const;
};

template<>
struct UDeserialize<std::string>
{
	void operator()(const nlohmann::json& json, std::string& v) const;
};

template<>
struct UDeserialize<UPath>
{
	void operator()(const nlohmann::json& json, UPath& v) const;
};

template<typename T>
	requires std::is_enum_v<T>
struct UDeserialize<T>
{
	void operator()(const nlohmann::json& json, T& v) const
	{
		using enum_t = std::underlying_type_t<T>;
		enum_t value{ static_cast<enum_t>(v) };
		UDeserialize<enum_t>{}(json, value);
		v = static_cast<T>(value);
	}
};

template <typename T>
concept Reservable = requires(T & v, std::size_t n)
{
	v.reserve(n);
};

template <typename T>
concept BackInsertable = requires(T & v, std::ranges::range_value_t<T> val)
{
	v.push_back(val);
};

template<typename T>
	requires std::ranges::range<T>
          && requires(T& container, std::ranges::range_value_t<T> val) { container.push_back(val); }
          && (!std::convertible_to<T, std::string>)
struct UDeserialize<T>
{
	void operator()(const nlohmann::json& json, T& v) const
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
}; 

template<std::derived_from<AAsset> T>
struct UDeserialize<T>
{
	void operator()(const nlohmann::json& json, T* v) const
	{
		UPath path{};
		UDeserialize<UPath>{}(json, path);
		v = SAssetManager<T>::Get(path);
	}
};
