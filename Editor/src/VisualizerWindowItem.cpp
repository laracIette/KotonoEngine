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
    return new WRow({
        .children = {
            new WBox({
                .size = { 25.0f, 25.0f },
                .child = new WStack({
                    .children = {
                        isFieldVisible 
                            ? new WColor({ Colors::Green }) 
                            : new WColor({ Colors::Red }),
                        new WButton({
                            .onPressed = [this, isFieldVisible]() {
                                SetState([this, isFieldVisible]() {
                                    Visualizer.SetIsFieldVisible(field_, !isFieldVisible);
                                });
                            },
                        }),
                    },
                }),
            }),
            new WText({ 
                .text = name_,
                .fontSize = { 20.0f, 25.0f },
                .spacing = -8.0f,
            }),
        },
    });
}

#include "generated/VisualizerWindowItem.generated.inl"
