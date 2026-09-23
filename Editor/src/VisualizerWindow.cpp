#include "VisualizerWindow.h"

#include "VisualizerWindowItem.h"
#include <kotono_interface/widgets.h>

WidgetPtr WVisualizerWindow::Build()
{
    return (
        UCreate<WBox>{}()
        | Apply(&WBox::SetSize, glm::vec2{ 400.0f, 300.0f })
        | (
            UCreate<WStack>{}()
            | (
                UCreate<WColor>{}()
                | Apply(&WColor::SetColor, Colors::Blue.WithAlpha(0.5f))
            )
            | (
                UCreate<WPadding>{}()
                | Apply(&WPadding::SetPadding, UPadding::All(8.0f))
                | (
                    UCreate<WColumn>{}()
                    | Apply(&WColumn::SetSpacing, 10.0f)
                    | (
                        UCreate<WWrap>{}()
                        | (
                            UCreate<WStack>{}()
                            | (
                                UCreate<WColor>{}()
                                | Apply(&WColor::SetColor, Colors::Black.WithAlpha(0.5f))
                            )
                            | (
                                UCreate<WText>{}()
                                | Apply(&WText::SetText, "Visualizer")
                            )
                        )
                    )
                    | (
                        UCreate<WList>{}()
                        | Apply(&WList::SetSpacing, 5.0f)
                        | (
                            UCreate<WVisualizerWindowItem>{}(EVisualizationField::SceneObject, "Scene Object")
                        )
                        | (
                            UCreate<WVisualizerWindowItem>{}(EVisualizationField::SceneObjectBounds, "Scene Object Bounds")
                        )
                        | (
                            UCreate<WVisualizerWindowItem>{}(EVisualizationField::SceneObjectCollider, "Scene Object Collider")
                        )
                        | (
                            UCreate<WVisualizerWindowItem>{}(EVisualizationField::SceneObjectWireframe, "Scene Object Wireframe")
                        )
                        | (
                            UCreate<WVisualizerWindowItem>{}(EVisualizationField::InterfaceObject, "Interface Object")
                        )
                        | (
                            UCreate<WVisualizerWindowItem>{}(EVisualizationField::InterfaceObjectBounds, "Interface Object Bounds")
                        )
                        | (
                            UCreate<WVisualizerWindowItem>{}(EVisualizationField::InterfaceObjectCollider, "Interface Object Collider")
                        )
                        | (
                            UCreate<WVisualizerWindowItem>{}(EVisualizationField::InterfaceObjectWireframe, "Interface Object Wireframe")
                        )
                    )
                )
            )
        )
    );
}

#include "generated/VisualizerWindow.generated.inl"
