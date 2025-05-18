#include "InterfaceTextComponent.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Font.h>
#include <kotono_framework/Shader.h>
#include "InterfaceObject.h"
#include "InterfaceStackComponent.h"
#include "InterfaceImageComponent.h"

void KInterfaceTextComponent::Construct()
{
    Base::Construct();

    letters_ = GetOwner()->AddComponent<KInterfaceStackComponent>();
}

void KInterfaceTextComponent::Init()
{
    letters_->SetOrientation(EOrientation::Horizontal);
}

const std::string& KInterfaceTextComponent::GetText() const
{
    return text_;
}

const float KInterfaceTextComponent::GetFontSize() const
{
    return fontSize_;
}

const float KInterfaceTextComponent::GetSpacing() const
{
    return letters_->GetItemSpacing();
}

KtShader* KInterfaceTextComponent::GetShader() const
{
    return shader_;
}

void KInterfaceTextComponent::SetText(const std::string& text)
{
    text_ = text;
    UpdateLetters();
}

void KInterfaceTextComponent::SetFontSize(const float fontSize)
{
    fontSize_ = fontSize;
}

void KInterfaceTextComponent::SetSpacing(const float spacing)
{
    letters_->SetItemSpacing(spacing);
}

void KInterfaceTextComponent::SetShader(KtShader* shader)
{
    shader_ = shader;
}

void KInterfaceTextComponent::UpdateLetters()
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
        auto* letter = GetOwner()->AddComponent<KInterfaceImageComponent>();
        letter->SetImageTexture(texture);
        letter->SetShader(shader_);
        letter->GetRect().SetScreenSize(glm::vec2(fontSize_));
        letters_->AddItem(letter);
    }
}
