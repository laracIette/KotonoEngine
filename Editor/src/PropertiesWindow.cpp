#include "PropertiesWindow.h"
#include "ObjectProperties.h"
#include <kotono_common/log.h>
#include <kotono_core/InterfaceComponent.h>
#include <kotono_core/InterfaceObject.h>
#include <kotono_core/ObjectManager.h>
#include <kotono_core/SceneComponent.h>
#include <kotono_core/SceneObject.h>
#include <kotono_input/Mouse.h>
#include <kotono_interface/widgets.h>
#include <kotono_platform/glm_utils.h>

WWidget* WPropertiesWindow::Build()
{

    ObjectManager.EventSelectedObjectChanged().AddListener(this, &WPropertiesWindow::Refresh);

    UPtr selectedObject{ TryCast<TSceneObject>(ObjectManager.GetSelectedObject()) };

    if (!selectedObject)
    {
		return nullptr;
    }
  
    return new WWrap({
        .child = new WStack({
            .children = {
                new WColor({ UColor::Blue().WithAlpha(0.5f) }),
                new WPadding({
                    .padding = WPadding::Padding::All(8.0f),
                    .child = new WList({
                        .spacing = 10.0f,
                        .children = [this, selectedObject]() {
						    return WidgetVector{
                                new WWrap({
                                    .child = new WStack({
                                        .children = {
                                            new WColor({ UColor::Black().WithAlpha(0.5f) }),
                                            new WText({
                                                .text = "Properties",
                                                .spacing = -20.0f,
                                            }),
                                        },
                                    }),
                                }),

                                new WObjectProperties(selectedObject),

                                new WColumn({
                                    .spacing = 10.0f,
                                    .children = {
                                        Slider("Position X", [selectedObject](const float delta) {
								            if (selectedObject) selectedObject->RootComponent()->Translate({ delta * 0.01f, 0.0f, 0.0f });
                                        }),
                                        Slider("Position Y", [selectedObject](const float delta) {
                                            if (selectedObject) selectedObject->RootComponent()->Translate({ 0.0f, delta * 0.01f, 0.0f });
                                        }),
                                        Slider("Position Z", [selectedObject](const float delta) {
                                            if (selectedObject) selectedObject->RootComponent()->Translate({ 0.0f, 0.0f, delta * 0.01f });
                                        }),

                                        Slider("Scale X", [selectedObject](const float delta) {
                                            if (selectedObject) selectedObject->RootComponent()->Scale({ 1.0f + delta * 0.001f, 1.0f, 1.0f });
                                        }),
                                        Slider("Scale Y", [selectedObject](const float delta) {
                                            if (selectedObject) selectedObject->RootComponent()->Scale({ 1.0f, 1.0f + delta * 0.001f, 1.0f });
                                        }),
                                        Slider("Scale Z", [selectedObject](const float delta) {
                                            if (selectedObject) selectedObject->RootComponent()->Scale({ 1.0f, 1.0f, 1.0f + delta * 0.001f });
                                        }),

                                        Slider("Rotation Pitch", [selectedObject](const float delta) {
                                            if (selectedObject) selectedObject->RootComponent()->Rotate(glm::angleAxis(delta * 0.001f, WorldRightVector));
                                        }),
                                        Slider("Rotation Yaw", [selectedObject](const float delta) {
                                            if (selectedObject) selectedObject->RootComponent()->Rotate(glm::angleAxis(delta * 0.001f, WorldUpVector));
                                        }),
                                        Slider("Rotation Roll", [selectedObject](const float delta) {
                                            if (selectedObject) selectedObject->RootComponent()->Rotate(glm::angleAxis(delta * 0.001f, WorldForwardVector));
                                        }),
                                    },
                                }),
                            };
                        },
                    }),
                }),
            },
        }),
    });
}

void WPropertiesWindow::Cleanup()
{
    ObjectManager.EventSelectedObjectChanged().RemoveListener(this, &WPropertiesWindow::Refresh);

    WWidget::Cleanup();
}

WWidget* WPropertiesWindow::Slider(const std::string& label, const ValueChangedFunction& function)
{
    return new WWrap({
        .axis = WWrap::Axis::Vertical,
        .child = new WStack({
            .children = {
                new WColor({ UColor::White().WithValue(0.5f) }),
                new WText({
                    .text = label,
                    .fontSize = { 20.0f, 25.0f },
                    .spacing = -8.0f,
                }),
                new WButton({
                    .onDown = [function]() {
                        const float delta{ Mouse.CursorPositionDelta().x };
                        if (delta != 0.0f)
                        {
                            function(delta);
                        }
                    },
                }),
            },
        }),
    });
}
