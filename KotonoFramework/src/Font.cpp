#include "Font.h"
#include "Framework.h"
#include "ImageTextureManager.h"
#include "ImageTexture.h"

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

    for (const auto letter : text)
    {
        const auto letterPath = path_ / std::format("{}.png", letter);
        if (std::filesystem::exists(letterPath))
        {
            result.push_back(Framework.GetImageTextureManager().Create(letterPath));
        }
    }

    return result;
}
