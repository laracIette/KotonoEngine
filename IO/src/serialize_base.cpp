#include "serialize_base.h"
#include <kotono_common/Path.h>
#include <nlohmann/json.hpp>

b8 contains(nlohmann::json const& json, std::string_view name)
{
    return json.contains(name);
}

nlohmann::json& get(nlohmann::json& json, std::string_view name)
{
    return json[name];
}

nlohmann::json const& get(nlohmann::json const& json, std::string_view name)
{
    return json.at(name);
}

void make_array(nlohmann::json& json)
{
    json = nlohmann::json::array({});
}

nlohmann::json& get_next(nlohmann::json& json)
{
    return json.emplace_back();
}

size get_size(nlohmann::json const& json)
{
    return json.size();
}

nlohmann::json const& get_at(nlohmann::json const& json, size index)
{
    return json.at(index);
}

void USerialize<b8>::operator()(nlohmann::json& json, b8 const& v) const
{
    json = v;
}

void USerialize<i8>::operator()(nlohmann::json& json, i8 const& v) const
{
    json = v;
}

void USerialize<i16>::operator()(nlohmann::json& json, i16 const& v) const
{
    json = v;
}

void USerialize<i32>::operator()(nlohmann::json& json, i32 const& v) const
{
    json = v;
}

void USerialize<i64>::operator()(nlohmann::json& json, i64 const& v) const
{
    json = v;
}

void USerialize<u8>::operator()(nlohmann::json& json, u8 const& v) const
{
    json = v;
}

void USerialize<u16>::operator()(nlohmann::json& json, u16 const& v) const
{
    json = v;
}

void USerialize<u32>::operator()(nlohmann::json& json, u32 const& v) const
{
    json = v;
}

void USerialize<u64>::operator()(nlohmann::json& json, u64 const& v) const
{
    json = v;
}

void USerialize<f32>::operator()(nlohmann::json& json, f32 const& v) const
{
    json = v;
}

void USerialize<f64>::operator()(nlohmann::json& json, f64 const& v) const
{
    json = v;
}

void USerialize<std::string>::operator()(nlohmann::json& json, std::string const& v) const
{
    json = v;
}

void USerialize<UPath>::operator()(nlohmann::json& json, UPath const& v) const
{
    json = v.ToString();
}

void UDeserialize<b8>::operator()(nlohmann::json const& json, b8& v) const
{
    v = json;
}

void UDeserialize<i8>::operator()(nlohmann::json const& json, i8& v) const
{
    v = json;
}

void UDeserialize<i16>::operator()(nlohmann::json const& json, i16& v) const
{
    v = json;
}

void UDeserialize<i32>::operator()(nlohmann::json const& json, i32& v) const
{
    v = json;
}

void UDeserialize<i64>::operator()(nlohmann::json const& json, i64& v) const
{
    v = json;
}

void UDeserialize<u8>::operator()(nlohmann::json const& json, u8& v) const
{
    v = json;
}

void UDeserialize<u16>::operator()(nlohmann::json const& json, u16& v) const
{
    v = json;
}

void UDeserialize<u32>::operator()(nlohmann::json const& json, u32& v) const
{
    v = json;
}

void UDeserialize<u64>::operator()(nlohmann::json const& json, u64& v) const
{
    v = json;
}

void UDeserialize<f32>::operator()(nlohmann::json const& json, f32& v) const
{
    v = json;
}

void UDeserialize<f64>::operator()(nlohmann::json const& json, f64& v) const
{
    v = json;
}

void UDeserialize<std::string>::operator()(nlohmann::json const& json, std::string& v) const
{
    v = json;
}

void UDeserialize<UPath>::operator()(nlohmann::json const& json, UPath& v) const
{
    v = json.get<std::string>();
}
