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
                UCreate<WAlign>{}()
                | Apply(&WAlign::SetAlignment, UAlignment::Left())
                | (
                    UCreate<WBox>{}()
                    | Apply(&WBox::SetSize, glm::vec2{ 16.0f, 16.0f })
                    | (
                        UCreate<WButton>{}()
                        | Apply(&WButton::SetIsActivatable, true)
                        | Apply(&WButton::SetIsActivated, isFieldVisible_)
                        | Apply(&WButton::SetNormalColor, Colors::Red)
                        | Apply(&WButton::SetActivatedColor, Colors::Green)
                        | Apply(&WButton::SetOnActivated, [this]() {
                            if (onVisibilityChanged_)
                            {   
                                onVisibilityChanged_(field_, true);
                            }
                        })
                        | Apply(&WButton::SetOnDeactivated, [this]() {
                            if (onVisibilityChanged_)
                            {   
                                onVisibilityChanged_(field_, false);
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
