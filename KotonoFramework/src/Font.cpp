#include "Font.h"
#include "Framework.h"
#include "ImageTextureManager.h"
#include "ImageTexture.h"
#include "Path.h"
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
    {' ', "space"}
};

KtFont::KtFont(const std::filesystem::path& path) :
    path_(path)
{
}

const std::filesystem::path& KtFont::GetPath() const
{
    return path_;
}

const float KtFont::GetSize() const
{
    return size_;
}

const float KtFont::GetSpacing() const
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

const std::vector<KtImageTexture*> KtFont::GetTextTextures(const std::string_view text) const
{
    std::vector<KtImageTexture*> result;
    result.reserve(text.size());

    for (const auto character : text)
    {
        // default texture
        auto characterPath = Framework.GetPath().GetSolutionPath() / R"(assets\textures\white_texture.jpg)";
        
        const auto it = CHARACTER_NAMES.find(character);
        if (it != CHARACTER_NAMES.end())
        {
            const auto fontCharacterPath = path_ / std::format("{}.png", it->second);
            if (std::filesystem::exists(fontCharacterPath))
            {
                // actual character texture if found
                characterPath = fontCharacterPath;
            }
        }

        result.push_back(Framework.GetImageTextureManager().Get(characterPath));
    }

    return result;
}
