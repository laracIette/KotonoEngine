#include "Path/Path.h"

#include "Path/PathManager.h"
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

UPath::UPath()
	: source_{}
{
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

UPath::UPath(UPath const& path)
	: source_{ path.source_ }
{
}

UPath::UPath(UPath&& path)
	: source_{ std::move(path.source_) }
{
}

UPath::UPath(std::filesystem::path const& source)
    : source_{ source.string() }
{
}

auto UPath::Directory() const -> UPath
{
    return ToPath().parent_path();
}

auto UPath::Name() const -> std::string
{
    return ToPath().filename().string();
}

auto UPath::Extension() const -> std::string
{
    return ToPath().extension().string();
}

auto UPath::Stem() const -> std::string
{
    return ToPath().stem().string();
}

auto UPath::HasDirectory() const -> b8
{
    return ToPath().has_parent_path();
}

auto UPath::IsEmpty() const -> b8
{
    return source_.empty();
}

auto UPath::IsFile() const -> b8
{
    return is_regular_file(ToPath());
}

auto UPath::Exists() const -> b8
{
    return exists(ToPath());
}

auto UPath::ToString() const -> std::string const&
{
    return source_;
}

auto UPath::ToPath() const -> std::filesystem::path
{
    std::string result{ source_ };
    replace(result, "${ENGINE_DIRECTORY}", SPathManager::Engine().ToString());
    replace(result, "${PROJECT_DIRECTORY}", SPathManager::Project().ToString());
    return result;
}

UPath& UPath::operator=(UPath const& other)
{
    if (other != *this)
    {
        source_ = other.source_;
    }
    return *this;
}

UPath& UPath::operator=(UPath&& other)
{
    source_ = std::move(other.source_);
    return *this;
}

UPath::operator std::string() const
{
    return ToString();
}

UPath::operator std::filesystem::path() const
{
    return ToPath();
}

UPath::operator b8() const
{
    return !IsEmpty();
}

auto UPath::operator+(char const* string) const -> UPath
{
    return std::format("{0}{1}", source_, string);
}

auto UPath::operator+(std::string_view string) const -> UPath
{
    return std::format("{0}{1}", source_, string);
}

auto UPath::operator==(UPath const& other) const noexcept -> b8
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
