#include "PropertiesWindow.h"
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
    ObjectManager.EventSelectedObjectChanged().AddListener(KtDelegate(this, &WPropertiesWindow::Refresh));

    if (UPtr selectedObject{ TryCast<TSceneObject>(ObjectManager.GetSelectedObject()) })
    {
        return new WStack({
            .children = {
                new WColor({ KtColor::Blue().WithAlpha(0.5f) }),
                new WPadding({
                    .padding = WPadding::Padding::All(8.0f),
                    .child = new WList({
                        .spacing = 10.0f,
                        .children = [this, selectedObject]() mutable {
							return WidgetVector{
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
                                Slider("Position X", [selectedObject](const float delta) mutable {
								    selectedObject->RootComponent()->Translate({ delta * 0.01f, 0.0f, 0.0f });
                                }),
                                Slider("Position Y", [selectedObject](const float delta) mutable {
                                    selectedObject->RootComponent()->Translate({ 0.0f, delta * 0.01f, 0.0f });
                                }),
                                Slider("Position Z", [selectedObject](const float delta) mutable {
                                    selectedObject->RootComponent()->Translate({ 0.0f, 0.0f, delta * 0.01f });
                                }),

                                Slider("Scale X", [selectedObject](const float delta) mutable {
								    selectedObject->RootComponent()->Scale({ 1.0f + delta * 0.001f, 1.0f, 1.0f });
                                }),
                                Slider("Scale Y", [selectedObject](const float delta) mutable {
                                    selectedObject->RootComponent()->Scale({ 1.0f, 1.0f + delta * 0.001f, 1.0f });
                                }),
                                Slider("Scale Z", [selectedObject](const float delta) mutable {
                                    selectedObject->RootComponent()->Scale({ 1.0f, 1.0f, 1.0f + delta * 0.001f });
                                }),

                                Slider("Rotation Pitch", [selectedObject](const float delta) mutable {
								    selectedObject->RootComponent()->Rotate(glm::angleAxis(delta * 0.001f, WorldRightVector));
                                }),
                                Slider("Rotation Yaw", [selectedObject](const float delta) mutable {
                                    selectedObject->RootComponent()->Rotate(glm::angleAxis(delta * 0.001f, WorldUpVector));
                                }),
                                Slider("Rotation Roll", [selectedObject](const float delta) mutable {
                                    selectedObject->RootComponent()->Rotate(glm::angleAxis(delta * 0.001f, WorldForwardVector));
                                }),
                            };
                        },
                    }),
                }),
            },
        });
    }

    return nullptr;
}

void WPropertiesWindow::Cleanup()
{
    ObjectManager.EventSelectedObjectChanged().RemoveListener(KtDelegate(this, &WPropertiesWindow::Refresh));

    WWidget::Cleanup();
}

WWidget* WPropertiesWindow::Slider(const std::string& label, const ValueChangedFunction& function)
{
    return new WBox({
        .size = { 250.0f, 25.0f },
        .child = new WStack({
            .children = {
                new WColor({ KtColor::White().WithValue(0.5f) }),
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
