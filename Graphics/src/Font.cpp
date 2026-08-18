#include "Font.h"
#include <kotono_common/AssetManager.h>
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

UFont::UFont(UPath const& path) 
    : path_{ path }
    , size_{ 10.0f }
    , spacing_{ 0.0f }
{
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
    std::vector<UPath> result{};
    result.reserve(text.size());

    for (auto const character : text)
    {
        // default texture
        UPath characterPath{ "${ENGINE_DIRECTORY}/Graphics/assets/textures/white_texture.jpg" };

        auto const it{ CHARACTER_NAMES.find(character) };
        if (it != CHARACTER_NAMES.end())
        {
            auto const fontCharacterPath{ path_ / std::format("{}.png", it->second) };
            if (std::filesystem::exists(fontCharacterPath))
            {
                // actual character texture if found
                characterPath = fontCharacterPath;
            }
        }

        result.push_back(characterPath);
    }

    return result;
}
