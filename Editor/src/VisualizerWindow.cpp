#include "VisualizerWindow.h"
#include <kotono_interface/widgets.h>
#include "Editor.h"
#include "Visualizer.h"
#include "log.h"

WWidget* WVisualizerWindow::Build()
{
    return new WBox({
        .size = { 800.0f, 400.0f },
        .child = new WStack({
            .children = {
                new WColor({ KtColor::Blue().WithAlpha(0.5f) }),
                new WPadding({
                    .padding = WPadding::Padding::All(8.0f),
                    .child = new WColumn({
                        .spacing = 10.0f,
                        .children = {
                            new WStack({
                                .children = {
                                    new WText({
                                        .text = "Visualizer",
                                        .spacing = -20.0f,
                                    }),
                                    new WBox({
                                        .size = { 400.0f, 60.0f },
                                        .child = new WColor({ KtColor::Black().WithAlpha(0.5f) }),
                                    }),
                                },
                            }),
                            new WColumn({
                                .spacing = 5.0f,
                                .children = {
                                    BuildVisualizationFieldWidget(EVisualizationField::SceneObject, "Scene Object"),
                                    BuildVisualizationFieldWidget(EVisualizationField::SceneObjectBounds, "Scene Object Bounds"),
                                    BuildVisualizationFieldWidget(EVisualizationField::SceneObjectCollider, "Scene Object Collider"),
                                    BuildVisualizationFieldWidget(EVisualizationField::SceneObjectWireframe, "Scene Object Wireframe"),
                                    BuildVisualizationFieldWidget(EVisualizationField::InterfaceObject, "Interface Object"),
                                    BuildVisualizationFieldWidget(EVisualizationField::InterfaceObjectBounds, "Interface Object Bounds"),
                                    BuildVisualizationFieldWidget(EVisualizationField::InterfaceObjectCollider, "Interface Object Collider"),
                                    BuildVisualizationFieldWidget(EVisualizationField::InterfaceObjectWireframe, "Interface Object Wireframe"),
                                },
                            }),
                        },
                    }),
                }),
            },
        }),
    });
}

WWidget* WVisualizerWindow::BuildVisualizationFieldWidget(const EVisualizationField field, const std::string& name)
{
    const bool isFieldVisible{ Editor.Visualizer().GetIsFieldVisible(field) };
    return new WRow({
        .children = {
            new WBox({
                .size = { 25.0f, 25.0f },
                .child = new WStack({
                    .children = {
                        isFieldVisible 
                            ? new WColor({ KtColor::Green() }) 
                            : new WColor({ KtColor::Red() }),
                        new WButton({
                            .onPress = [this, field, isFieldVisible]() {
                                SetState([field, isFieldVisible]() {
                                    Editor.Visualizer().SetIsFieldVisible(field, !isFieldVisible);
                                });
                            },
                        }),
                    },
                }),
            }),
            new WText({ 
                .text = name,
                .fontSize = { 20.0f, 25.0f },
                .spacing = -8.0f,
            }),
        },
    });
}