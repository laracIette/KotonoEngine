#include "InterfaceTextComponent.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Font.h>
#include <kotono_framework/Shader.h>
#include "InterfaceObject.h"
#include "InterfaceStackComponent.h"
#include "InterfaceImageComponent.h"
#include "Timer.h"
#include "Engine.h"
#include "ObjectManager.h"

KInterfaceTextComponent::KInterfaceTextComponent(RInterfaceObject* owner) :
    Base(owner)
{
    updateTextWithBindingTimer_ = Engine.GetObjectManager().Create<KTimer>();
}

void KInterfaceTextComponent::Init()
{
    Base::Init();

    SetOrientation(EOrientation::Horizontal);

    updateTextWithBindingTimer_->SetDuration(1.0f / 30.0f);
    updateTextWithBindingTimer_->SetIsRepeat(true);
    updateTextWithBindingTimer_->GetEventCompleted().AddListener(KtDelegate(this, &KInterfaceTextComponent::UpdateTextWithBinding));
    updateTextWithBindingTimer_->Start();
}

void KInterfaceTextComponent::Cleanup()
{
    Base::Cleanup();

    updateTextWithBindingTimer_->Delete();
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
    return GetItemSpacing();
}

KtShader* KInterfaceTextComponent::GetShader() const
{
    return shader_;
}

void KInterfaceTextComponent::SetText(const std::string& text)
{
    if (text_ == text)
    {
        return;
    }

    text_ = text;
    UpdateCharacters();
}

void KInterfaceTextComponent::SetFontSize(const float fontSize)
{
    fontSize_ = fontSize;
}

void KInterfaceTextComponent::SetSpacing(const float spacing)
{
    SetItemSpacing(spacing);
}

void KInterfaceTextComponent::SetShader(KtShader* shader)
{
    shader_ = shader;
}

void KInterfaceTextComponent::SetTextBinding(const TextBinding& function)
{
    textBinding_ = function;
}

void KInterfaceTextComponent::UpdateTextWithBinding()
{
    if (textBinding_)
    {
        SetText(textBinding_());
    }
}

void KInterfaceTextComponent::UpdateCharacters()
{
    for (auto* letter : GetItems())
    {
        letter->Delete();
    }
    ClearItems();

    const auto path = Framework.GetPath().GetSolutionPath() / R"(assets\fonts\default)";
    const auto font = KtFont(path);
    for (auto* texture : font.GetTextTextures(text_))
    {
        auto* letter = GetOwner()->AddComponent<KInterfaceImageComponent>();
        letter->SetImageTexture(texture);
        letter->SetShader(shader_);
        letter->SetScreenSize(glm::vec2(fontSize_));
        AddItem(letter);
    }
}
