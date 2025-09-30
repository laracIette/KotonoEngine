#include "PropertiesWindow.h"
#include <kotono_interface/widgets.h>
#include <kotono_engine/Engine.h>
#include <kotono_engine/ObjectManager.h>
#include <kotono_engine/InterfaceObject.h>
#include <kotono_engine/InterfaceComponent.h>
#include <kotono_framework/Framework.h>
#include <kotono_framework/InputManager.h>

WWidget* WPropertiesWindow::Build()
{
    return new WStack({
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
                                    .text = "Properties",
                                    .spacing = -20.0f,
                                }),
                                new WBox({
                                    .size = { 400.0f, 60.0f },
                                    .child = new WColor({ KtColor::Black().WithAlpha(0.5f) }),
                                }),
                            },
                        }),
                        new WBox({
                            .size = { 250.0f, 25.0f },
                            .child = new WStack({
                                .children = {
                                    new WColor({ KtColor::White().WithValue(0.5f) }),
                                    new WText({
                                        .text = "Position.x",
                                        .fontSize = { 20.0f, 25.0f },
                                        .spacing = -8.0f,
                                    }),
                                    new WButton({
                                        .onDown = []() {
                                            const auto delta = Framework.InputManager().GetMouse().GetCursorPositionDelta().x;
                                            if (!delta)
                                            {
                                                return;
                                            }
                                            if (auto* asInterfaceObject = dynamic_cast<RInterfaceObject*>(Engine.ObjectManager().selectedObject))
                                            {
                                                asInterfaceObject->GetRootComponent()->Translate({ delta / 800.0f, 0.0f });
                                            }
                                        },
                                    }),
                                },
                            }),
                        }),
                    },
                }),
            }),
        },
        });
}
