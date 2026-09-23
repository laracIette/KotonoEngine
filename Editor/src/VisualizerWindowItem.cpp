#include "VisualizerWindowItem.h"

#include <kotono_interface/widgets.h>

WVisualizerWindowItem::WVisualizerWindowItem(EVisualizationField field, std::string_view name) 
    : field_{ field }
    , name_{ name }
{
}

WidgetPtr WVisualizerWindowItem::Build()
{
    b8 const isFieldVisible{ false };

    return (
        UCreate<WRow>{}()
        | (
            UCreate<WBox>{}()
            | Apply(&WBox::SetSize, glm::vec2{ 25.0f, 25.0f })
            | (
                UCreate<WStack>{}()
                | (
                    UCreate<WColor>{}()
                    | Apply(&WColor::SetColor, isFieldVisible ? Colors::Green : Colors::Red)
                )
                | (
                    UCreate<WButton>{}()
                    | Apply(&WButton::SetOnClicked, [this]() { throw std::runtime_error{ "unimplemented!" }; })
                )
            )
        )
        | (
            UCreate<WText>{}()
            | Apply(&WText::SetText, name_)
            | Apply(&WText::SetFontSize, glm::vec2{ 20.0f, 25.0f })
        )
    );
}

#include "generated/VisualizerWindowItem.generated.inl"
