#include "Path.h"
#include "PathManager.h"
#include <ranges>
#include <algorithm>

static constexpr void replace(std::string& str, std::string_view from, std::string_view to)
{
    const size start_pos{ str.find(from) };
    if (start_pos != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
    }
}

UPath::UPath(std::string const& source) 
    : source_{ source }
{
}

UPath::UPath(std::string&& source) 
    : source_{ std::move(source) }
{
}

UPath::UPath(char const* source)
    : source_{ source }
{
}

UPath::UPath(std::filesystem::path const& source)
    : source_{ source.string() }
{
}

UPath UPath::Directory() const
{
    return ToPath().parent_path();
}

std::string UPath::Name() const
{
    return ToPath().filename().string();
}

std::string UPath::Extension() const
{
    return ToPath().extension().string();
}

std::string UPath::Stem() const
{
    return ToPath().stem().string();
}

b8 UPath::IsEmpty() const
{
    return ToPath().empty();
}

b8 UPath::IsFile() const
{
    return is_regular_file(ToPath());
}

std::string const& UPath::ToString() const
{
    return source_;
}

std::filesystem::path UPath::ToPath() const
{
    std::string result{ source_ };
    replace(result, "${ENGINE_DIRECTORY}", SPathManager::Engine().string());
    replace(result, "${PROJECT_DIRECTORY}", SPathManager::Project().string());
    return result;
}

UPath::operator std::string() const
{
    return ToString();
}

UPath::operator std::filesystem::path() const
{
    return ToPath();
}

bool UPath::operator==(UPath const& other) const noexcept
{
    return source_ == other.source_;
}

size std::hash<UPath>::operator()(UPath const& p) const noexcept
{
    return std::hash<std::string>{}(p.source_);
}

UPath operator/(UPath const& r, UPath const& l)
{
    return std::format("{0}/{1}", r.source_, l.source_);
}
