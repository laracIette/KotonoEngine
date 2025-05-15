#include "Text.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Font.h>
#include <kotono_framework/Shader.h>
#include "Engine.h"
#include "ObjectManager.h"
#include "Image.h"
#include "InterfaceObjectStack.h"

void RText::Construct()
{
    Base::Construct();

    letters_ = Engine.GetObjectManager().Create<RInterfaceObjectStack>();
}

void RText::Init()
{
    letters_->SetOrientation(EOrientation::Horizontal);
}

const std::string& RText::GetText() const
{
    return text_;
}

const float RText::GetFontSize() const
{
    return fontSize_;
}

const float RText::GetSpacing() const
{
    return letters_->GetItemSpacing();;
}

KtShader* RText::GetShader() const
{
    return shader_;
}

void RText::SetText(const std::string& text)
{
    text_ = text;
    UpdateLetters();
}

void RText::SetFontSize(const float fontSize)
{
    fontSize_ = fontSize;
}

void RText::SetSpacing(const float spacing)
{
    letters_->SetItemSpacing(spacing);
}

void RText::SetShader(KtShader* shader)
{
    shader_ = shader;
}

void RText::UpdateLetters()
{
    for (auto* letter : letters_->GetItems())
    {
        letter->SetIsDelete(true);
    }
    letters_->ClearItems();

    const auto path = Framework.GetPath().GetSolutionPath() / R"(assets\fonts\default)";
    const auto font = KtFont(path);
    for (auto* texture : font.GetTextTextures(text_))
    {
        auto* letter = Engine.GetObjectManager().Create<RImage>();
        letter->SetImageTexture(texture);
        letter->SetShader(shader_);
        letter->GetRect().SetScreenSize(glm::vec2(fontSize_));
        letters_->AddItem(letter);
    }
}
