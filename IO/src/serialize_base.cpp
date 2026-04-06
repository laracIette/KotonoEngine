#include "serialize_base.h"
#include <nlohmann/json.hpp>
#include <kotono_common/Path.h>

nlohmann::json& get(nlohmann::json& json, const std::string_view name)
{
    return json[name];
}

const nlohmann::json& get(const nlohmann::json& json, const std::string_view name)
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

size get_size(const nlohmann::json& json)
{
    return json.size();
}

const nlohmann::json& get_at(const nlohmann::json& json, size index)
{
    return json.at(index);
}

void USerialize<bool>::operator()(nlohmann::json& json, const bool v) const
{
    json = v;
}

void USerialize<i8>::operator()(nlohmann::json& json, const i8 v) const
{
    json = v;
}

void USerialize<i16>::operator()(nlohmann::json& json, const i16 v) const
{
    json = v;
}

void USerialize<i32>::operator()(nlohmann::json& json, const i32 v) const
{
    json = v;
}

void USerialize<i64>::operator()(nlohmann::json& json, const i64 v) const
{
    json = v;
}

void USerialize<u8>::operator()(nlohmann::json& json, const u8 v) const
{
    json = v;
}

void USerialize<u16>::operator()(nlohmann::json& json, const u16 v) const
{
    json = v;
}

void USerialize<u32>::operator()(nlohmann::json& json, const u32 v) const
{
    json = v;
}

void USerialize<u64>::operator()(nlohmann::json& json, const u64 v) const
{
    json = v;
}

void USerialize<float>::operator()(nlohmann::json& json, const float v) const
{
    json = v;
}

void USerialize<double>::operator()(nlohmann::json& json, const double v) const
{
    json = v;
}

void USerialize<std::string>::operator()(nlohmann::json& json, const std::string& v) const
{
    json = v;
}

void USerialize<UPath>::operator()(nlohmann::json& json, const UPath& v) const
{
    json = v.ToString();
}


void UDeserialize<bool>::operator()(const nlohmann::json& json, bool& v) const
{
    v = json;
}

void UDeserialize<i8>::operator()(const nlohmann::json& json, i8& v) const
{
    v = json;
}

void UDeserialize<i16>::operator()(const nlohmann::json& json, i16& v) const
{
    v = json;
}

void UDeserialize<i32>::operator()(const nlohmann::json& json, i32& v) const
{
    v = json;
}

void UDeserialize<i64>::operator()(const nlohmann::json& json, i64& v) const
{
    v = json;
}

void UDeserialize<u8>::operator()(const nlohmann::json& json, u8& v) const
{
    v = json;
}

void UDeserialize<u16>::operator()(const nlohmann::json& json, u16& v) const
{
    v = json;
}

void UDeserialize<u32>::operator()(const nlohmann::json& json, u32& v) const
{
    v = json;
}

void UDeserialize<u64>::operator()(const nlohmann::json& json, u64& v) const
{
    v = json;
}

void UDeserialize<float>::operator()(const nlohmann::json& json, float& v) const
{
    v = json;
}

void UDeserialize<double>::operator()(const nlohmann::json& json, double& v) const
{
    v = json;
}

void UDeserialize<std::string>::operator()(const nlohmann::json& json, std::string& v) const
{
    v = json;
}

void UDeserialize<UPath>::operator()(const nlohmann::json& json, UPath& v) const
{
    v = json.get<std::string>();
}
