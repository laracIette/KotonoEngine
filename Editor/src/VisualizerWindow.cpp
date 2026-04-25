#include "VisualizerWindow.h"
#include "VisualizerWindowItem.h"
#include <kotono_interface/widgets.h>

WidgetPtr WVisualizerWindow::Build()
{
    UPtr bgColor{ Create<WColor>{}() };
    bgColor->SetColor(Colors::Blue.WithAlpha(0.5f));

    UPtr bgText{ Create<WColor>{}() };
    bgText->SetColor(Colors::Black.WithAlpha(0.5f));

    UPtr headerText{ Create<WText>{}() };
    headerText->SetText("Visualizer");
    headerText->SetSpacing(-20.0f);

    UPtr textStack{ Create<WStack>{}() };
    textStack->SetChildren({ bgText, headerText });

    UPtr textWrap{ Create<WWrap>{}() };
    textWrap->SetChild(textStack);

    UPtr itemList{ Create<WList>{}() };
    itemList->SetSpacing(5.0f);
    itemList->SetChildren({
        Create<WVisualizerWindowItem>{}(EVisualizationField::SceneObject, "Scene Object"),
        Create<WVisualizerWindowItem>{}(EVisualizationField::SceneObjectBounds, "Scene Object Bounds"),
        Create<WVisualizerWindowItem>{}(EVisualizationField::SceneObjectCollider, "Scene Object Collider"),
        Create<WVisualizerWindowItem>{}(EVisualizationField::SceneObjectWireframe, "Scene Object Wireframe"),
        Create<WVisualizerWindowItem>{}(EVisualizationField::InterfaceObject, "Interface Object"),
        Create<WVisualizerWindowItem>{}(EVisualizationField::InterfaceObjectBounds, "Interface Object Bounds"),
        Create<WVisualizerWindowItem>{}(EVisualizationField::InterfaceObjectCollider, "Interface Object Collider"),
        Create<WVisualizerWindowItem>{}(EVisualizationField::InterfaceObjectWireframe, "Interface Object Wireframe")
    });

    UPtr column{ Create<WColumn>{}() };
    column->SetSpacing(10.0f);
    column->SetChildren({ textWrap, itemList });

    UPtr padding{ Create<WPadding>{}() };
    padding->SetPadding(UPadding::All(8.0f));
    padding->SetChild(column);

    UPtr mainStack{ Create<WStack>{}() };
    mainStack->SetChildren({ bgColor, padding });

    UPtr box{ Create<WBox>{}() };
    box->SetChild(mainStack);
    box->SetSize({ 400.0f, 300.0f });

    return box;
}

#include "generated/VisualizerWindow.generated.inl"
