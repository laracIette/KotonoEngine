#include "VisualizerWindow.h"
#include "VisualizerWindowItem.h"
#include <kotono_interface/widgets.h>

WidgetPtr WVisualizerWindow::Build()
{
    return new WBox({
        .size = { 600.0f, 300.0f },
        .child = new WStack({
            .children = {
                new WColor({ Colors::Blue.WithAlpha(0.5f) }),
                new WPadding({
                    .padding = WPadding::Padding::All(8.0f),
                    .child = new WColumn({
                        .spacing = 10.0f,
                        .children = {
                            new WWrap({
                                .child = new WStack({
                                    .children = {
                                        new WColor({ Colors::Black.WithAlpha(0.5f) }),
                                        new WText({
                                            .text = "Visualizer",
                                            .spacing = -20.0f,
                                        }),
                                    },
                                }),
                            }),
                            new WList({
                                .spacing = 5.0f,
                                .children = [this]() {
                                    return WidgetVector{
                                        new WVisualizerWindowItem(EVisualizationField::SceneObject, "Scene Object"),
                                        new WVisualizerWindowItem(EVisualizationField::SceneObjectBounds, "Scene Object Bounds"),
                                        new WVisualizerWindowItem(EVisualizationField::SceneObjectCollider, "Scene Object Collider"),
                                        new WVisualizerWindowItem(EVisualizationField::SceneObjectWireframe, "Scene Object Wireframe"),
                                        new WVisualizerWindowItem(EVisualizationField::InterfaceObject, "Interface Object"),
                                        new WVisualizerWindowItem(EVisualizationField::InterfaceObjectBounds, "Interface Object Bounds"),
                                        new WVisualizerWindowItem(EVisualizationField::InterfaceObjectCollider, "Interface Object Collider"),
                                        new WVisualizerWindowItem(EVisualizationField::InterfaceObjectWireframe, "Interface Object Wireframe"),
                                    };
                                },
                            }),
                        },
                    }),
                }),
            },
        }),
    });
}
