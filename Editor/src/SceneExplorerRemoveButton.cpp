#include "SceneExplorerRemoveButton.h"
#include <kotono_core/Object.h>
#include <kotono_core/ObjectManager.h>
#include <kotono_interface/widgets.h>
#include <kotono_core/Game.h>
#include <kotono_core/Scene.h>
#include <kotono_core/SceneObject.h>

WidgetPtr WSceneExplorerRemoveButton::Build()
{
    ObjectManager.EventSelectedObjectChanged().AddListener(this, &WSceneExplorerRemoveButton::Refresh);

    return new WStack({
        .children = {
            ObjectManager.GetSelectedObject()
				? new WColor({ Colors::Red })
                : new WColor({ Colors::Red.WithValue(0.1f) }),
            new WButton({
                .onDown = [this]() {
                    if (UPtr selectedObject{ TryCast<TSceneObject>(ObjectManager.GetSelectedObject()) })
                    {
                        SetState([selectedObject]() {
                            if (UPtr scene{ Game.GetOpenedScene() })
                            {
                                scene->Remove(selectedObject);
                            }
                            ObjectManager.SetSelectedObject(nullptr);
                            selectedObject->Delete();
                        });
                    }
                },
            }),
        },
    });
}

void WSceneExplorerRemoveButton::Cleanup()
{
    ObjectManager.EventSelectedObjectChanged().RemoveListener(this, &WSceneExplorerRemoveButton::Refresh);

    WWidget::Cleanup();
}
