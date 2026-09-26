#include "SceneVisibilityField.h"

#include <kotono_interface/widgets.h>

WSceneVisibilityField::WSceneVisibilityField(ESceneVisibility field, std::string_view name, b8 isFieldVisible) 
    : field_{ field }
    , name_{ name }
    , isFieldVisible_{ isFieldVisible }
{
}

WidgetPtr WSceneVisibilityField::Build()
{
    return (
        UCreate<WWrap>{}()
        | (
            UCreate<WRow>{}()
            | (
                UCreate<WCenter>{}()
                | Apply(&WCenter::SetAxis, EAxis::Vertical)
                | (
                    UCreate<WBox>{}()
                    | Apply(&WBox::SetSize, glm::vec2{ 16.0f, 16.0f })
                    | (
                        UCreate<WButton>{}()
                        | Apply(&WButton::SetNormalColor, [this]() { return isFieldVisible_ ? Colors::Green : Colors::Red; })
                        | Apply(&WButton::SetOnClicked, [this]() {
                            if (onVisibilityChanged_)
                            {   
                                isFieldVisible_ = !isFieldVisible_;
                                onVisibilityChanged_(field_, isFieldVisible_);
                            }
                        })
                    )
                )
            )
            | (
                UCreate<WText>{}()
                | Apply(&WText::SetText, name_)
            )
        )
    );
}

#include "generated/SceneVisibilityField.generated.inl"
