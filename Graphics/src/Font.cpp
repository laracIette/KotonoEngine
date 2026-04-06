#include "Font.h"
#include "Texture.h"
#include <kotono_common/AssetManager.h>
#include <kotono_common/PathManager.h>
#include <unordered_map>

static const std::unordered_map<char, const char*> CHARACTER_NAMES =
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

KtFont::KtFont(const UPath& path) :
    path_(path)
{
}

const UPath& KtFont::Path() const
{
    return path_;
}

float KtFont::GetSize() const
{
    return size_;
}

float KtFont::GetSpacing() const
{
    return spacing_;
}

void KtFont::SetSize(const float size)
{
    size_ = size;
}

void KtFont::SetSpacing(const float spacing) 
{
    spacing_ = spacing;
}

std::vector<UPath> KtFont::GetTextPaths(const std::string_view text) const
{
    std::vector<UPath> result{};
    result.reserve(text.size());

    for (const auto character : text)
    {
        // default texture
        UPath characterPath("${ENGINE_DIRECTORY}/Graphics/assets/textures/white_texture.jpg");

        const auto it{ CHARACTER_NAMES.find(character) };
        if (it != CHARACTER_NAMES.end())
        {
            const auto fontCharacterPath{ path_ / std::format("{}.png", it->second) };
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

std::vector<KtTexture*> KtFont::GetTextTextures(const std::string_view text) const
{
    std::vector<KtTexture*> result;
    result.reserve(text.size());

    for (const auto& path : GetTextPaths(text))
    {
        result.push_back(UAssetManager<KtTexture>::Get(path));
    }

    return result;
}
