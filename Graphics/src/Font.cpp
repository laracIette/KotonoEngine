#include "Font.h"

#include <ranges>
#include <unordered_map>

static std::unordered_map<char, std::string_view> const CHARACTER_NAMES =
{
    {'a', "a"}, {'A', "ua"}, {'b', "b"}, {'B', "ub"},
    {'c', "c"}, {'C', "uc"}, {'d', "d"}, {'D', "ud"},
    {'e', "e"}, {'E', "ue"}, {'f', "f"}, {'F', "uf"},
    {'g', "g"}, {'G', "ug"}, {'h', "h"}, {'H', "uh"},
    {'i', "i"}, {'I', "ui"}, {'j', "j"}, {'J', "uj"},
    {'k', "k"}, {'K', "uk"}, {'l', "l"}, {'L', "ul"},
    {'m', "m"}, {'M', "um"}, {'n', "n"}, {'N', "un"},
    {'o', "o"}, {'O', "uo"}, {'p', "p"}, {'P', "up"},
    {'q', "q"}, {'Q', "uq"}, {'r', "r"}, {'R', "ur"},
    {'s', "s"}, {'S', "us"}, {'t', "t"}, {'T', "ut"},
    {'u', "u"}, {'U', "uu"}, {'v', "v"}, {'V', "uv"},
    {'w', "w"}, {'W', "uw"}, {'x', "x"}, {'X', "ux"},
    {'y', "y"}, {'Y', "uy"}, {'z', "z"}, {'Z', "uz"},
    {'0', "0"}, {'1', "1"}, {'2', "2"}, {'3', "3"}, {'4', "4"}, 
    {'5', "5"}, {'6', "6"}, {'7', "7"}, {'8', "8"}, {'9', "9"},
    {'#', "hash"}, {',', "comma"}, {'.', "dot"}, {':', "colon"}, {'\'', "apostrophe"}, 
    {'[', "lbracket"}, {']', "rbracket"}, {'{', "lbrace"}, {'}', "rbrace"}, 
    {'/', "fslash"}, {'\\', "bslash"}, 
    {'!', "exclamation"}, {'?', "question"}, 
    {'+', "plus"}, {'-', "minus"}, 
	{' ', "space"}, {'_', "underscore"},
	{'<', "lesser"}, {'>', "greater"},
};

static UPath const DEFAULT_TEXTURE{ "${ENGINE_DIRECTORY}/Graphics/assets/textures/white_texture.jpg" };

UFont::UFont(UPath const& path) 
    : path_{ path }
    , size_{ 10.0f }
    , spacing_{ 0.0f }
{
    characterPaths_ = std::vector<UPath>{ 256, DEFAULT_TEXTURE };

    for (auto const& [character, name] : CHARACTER_NAMES)
    {
        auto const fontCharacterPath{ path_ / std::format("{0}.png", name) };

        if (fontCharacterPath.Exists())
        {
            auto const index{ static_cast<size>(character) };
            characterPaths_[index] = fontCharacterPath;
        }
    }
}

UPath const& UFont::Path() const
{
    return path_;
}

f32 UFont::GetSize() const
{
    return size_;
}

f32 UFont::GetSpacing() const
{
    return spacing_;
}

void UFont::SetSize(f32 size)
{
    size_ = size;
}

void UFont::SetSpacing(f32 spacing) 
{
    spacing_ = spacing;
}

std::vector<UPath> UFont::GetTextPaths(std::string_view text) const
{
    return text
        | std::views::transform([this](char const character) { 
            return characterPaths_[static_cast<size>(character)]; 
        })
        | std::ranges::to<std::vector>();
}
