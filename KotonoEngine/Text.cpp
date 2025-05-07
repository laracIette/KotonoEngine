#include "Text.h"

const std::string& RText::GetText() const
{
    return text_;
}

void RText::SetText(const std::string& text)
{
    text_ = text;
    UpdateTexture();
}

void RText::UpdateTexture()
{
}
