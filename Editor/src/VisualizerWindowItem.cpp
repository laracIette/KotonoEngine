#include "VisualizerWindowItem.h"
#include "Visualizer.h"
#include <kotono_interface/widgets.h>

WVisualizerWindowItem::WVisualizerWindowItem(const EVisualizationField field, const std::string& name) 
    : field_(field)
    , name_(name)
{
}

WidgetPtr WVisualizerWindowItem::Build()
{
    const bool isFieldVisible{ Visualizer.GetIsFieldVisible(field_) };

    UPtr color{ Create<WColor>{}() };
    color->SetColor(isFieldVisible ? Colors::Green : Colors::Red);

    UPtr button{ Create<WButton>{}() };
    button->SetOnPressed([this, isFieldVisible]() {
        SetState([this, isFieldVisible]() {
            Visualizer.SetIsFieldVisible(field_, !isFieldVisible);
        });
    });

    UPtr stack{ Create<WStack>{}() };
    stack->SetChildren({ color, button });

    UPtr box{ Create<WBox>{}() };
    box->SetChild(stack);
    box->SetSize({ 25.0f, 25.0f });

    UPtr text{ Create<WText>{}() };
    text->SetText(name_);
    text->SetFontSize({ 20.0f, 25.0f });
    text->SetSpacing(-8.0f);

    UPtr row{ Create<WRow>{}() };
    row->SetChildren({ box, text });

    return row;
}

#include "generated/VisualizerWindowItem.generated.inl"
