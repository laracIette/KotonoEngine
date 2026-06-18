#include "VisualizerWindow.h"
#include "VisualizerWindowItem.h"
#include <kotono_interface/widgets.h>

WidgetPtr WVisualizerWindow::Build()
{
    UPtr bgColor{ UCreate<WColor>{}() };
    bgColor->SetColor(Colors::Blue.WithAlpha(0.5f));

    UPtr bgText{ UCreate<WColor>{}() };
    bgText->SetColor(Colors::Black.WithAlpha(0.5f));

    UPtr headerText{ UCreate<WText>{}() };
    headerText->SetText("Visualizer");
    headerText->SetSpacing(-20.0f);

    UPtr textStack{ UCreate<WStack>{}() };
    textStack->SetChildren({ bgText, headerText });

    UPtr textWrap{ UCreate<WWrap>{}() };
    textWrap->SetChild(textStack);

    UPtr itemList{ UCreate<WList>{}() };
    itemList->SetSpacing(5.0f);
    itemList->SetChildren({
        UCreate<WVisualizerWindowItem>{}(EVisualizationField::SceneObject, "Scene Object"),
        UCreate<WVisualizerWindowItem>{}(EVisualizationField::SceneObjectBounds, "Scene Object Bounds"),
        UCreate<WVisualizerWindowItem>{}(EVisualizationField::SceneObjectCollider, "Scene Object Collider"),
        UCreate<WVisualizerWindowItem>{}(EVisualizationField::SceneObjectWireframe, "Scene Object Wireframe"),
        UCreate<WVisualizerWindowItem>{}(EVisualizationField::InterfaceObject, "Interface Object"),
        UCreate<WVisualizerWindowItem>{}(EVisualizationField::InterfaceObjectBounds, "Interface Object Bounds"),
        UCreate<WVisualizerWindowItem>{}(EVisualizationField::InterfaceObjectCollider, "Interface Object Collider"),
        UCreate<WVisualizerWindowItem>{}(EVisualizationField::InterfaceObjectWireframe, "Interface Object Wireframe"),
    });

    UPtr column{ UCreate<WColumn>{}() };
    column->SetSpacing(10.0f);
    column->SetChildren({ textWrap, itemList });

    UPtr padding{ UCreate<WPadding>{}() };
    padding->SetPadding(UPadding::All(8.0f));
    padding->SetChild(column);

    UPtr mainStack{ UCreate<WStack>{}() };
    mainStack->SetChildren({ bgColor, padding });

    UPtr box{ UCreate<WBox>{}() };
    box->SetChild(mainStack);
    box->SetSize({ 400.0f, 300.0f });

    return box;
}

#include "generated/VisualizerWindow.generated.inl"
