#include "InterfaceTextComponent.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Font.h>
#include <kotono_framework/Shader.h>
#include <kotono_framework/ShaderManager.h>
#include "InterfaceObject.h"
#include "InterfaceImageComponent.h"
#include "Engine.h"

KInterfaceTextComponent::KInterfaceTextComponent(RInterfaceObject* owner) :
    Base(owner),
    fontSize_(32.0f),
    spacing_(0.04f)
{
}

void KInterfaceTextComponent::Init()
{
    Base::Init(); 

    static const auto path{ Framework.GetPath().GetFrameworkPath() / R"(shaders\shader2D.ktshader)" };
    SetShader(Framework.GetShaderManager().Get(path));

    UpdateTextWithBinding();
}

void KInterfaceTextComponent::Cleanup()
{
    Base::Cleanup();
}

const std::string& KInterfaceTextComponent::GetText() const
{
    return text_;
}

float KInterfaceTextComponent::GetFontSize() const
{
    return fontSize_;
}

float KInterfaceTextComponent::GetSpacing() const
{
    return spacing_;
}

KtShader* KInterfaceTextComponent::GetShader() const
{
    return shader_;
}

void KInterfaceTextComponent::SetText(const std::string_view text)
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
    spacing_ = spacing;
}

void KInterfaceTextComponent::SetShader(KtShader* shader)
{
    if (shader_ == shader)
    {
        return;
    }

    shader_ = shader;
    for (auto* character : characters_)
    {
        character->SetShader(shader_);
    }
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
    // delete old characters
    for (auto* character : characters_)
    {
        character->Delete();
    }
    characters_.Clear();

    const auto path{ Framework.GetPath().GetSolutionPath() / R"(assets\fonts\default)" };
    const KtFont font(path);

    const auto fontCharacters{ font.GetTextTextures(text_) };
    characters_.Reserve(fontCharacters.size());

    // fill with new characters
    for (auto* texture : fontCharacters)
    {
        auto* character{ GetOwner()->AddComponent<KInterfaceImageComponent>() };
        character->SetImageTexture(texture);
        character->SetShader(shader_);
        character->SetScreenSize(glm::vec2(fontSize_));
        characters_.Add(character); 
    }

    // offset characters
    for (size_t i{ 0 }; i < characters_.Size(); i++)
    {
        const float offset{ spacing_ * i };
        characters_[i]->Translate({ offset, 0.0f });
    }
}
