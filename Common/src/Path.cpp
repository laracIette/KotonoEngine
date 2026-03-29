#include "Path.h"
#include "PathManager.h"
#include <ranges>
#include <algorithm>

static void replace(std::string& str, const std::string& from, const std::string& to)
{
    const size start_pos{ str.find(from) };
    if (start_pos != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
    }
}

UPath::UPath() 
    : source_("")
{
}

UPath::UPath(const std::string& source) 
    : source_(source)
{
}

UPath::UPath(std::string&& source) 
    : source_(std::move(source))
{
}

UPath::UPath(const char* source) 
    : source_(source)
{
}

UPath::UPath(const std::filesystem::path& source)
    : source_(source.string())
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

bool UPath::IsEmpty() const
{
    return ToPath().empty();
}

bool UPath::IsFile() const
{
    return is_regular_file(ToPath());
}

std::string UPath::ToString() const
{
    return source_;
}

std::filesystem::path UPath::ToPath() const
{
    std::string result{ source_ };
    replace(result, "${ENGINE_DIRECTORY}", PathManager.Engine().string());
    replace(result, "${PROJECT_DIRECTORY}", PathManager.Project().string());
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

bool UPath::operator==(const UPath& other) const noexcept
{
    return source_ == other.source_;
}

size std::hash<UPath>::operator()(const UPath& p) const noexcept
{
    return std::hash<std::string>{}(p.source_);
}

UPath operator/(const UPath& r, const UPath& l)
{
    return std::format("{0}/{1}", r.source_, l.source_);
}
