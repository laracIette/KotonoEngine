#include "TestBox.h"
#include <kotono_core/Object.h>
#include <kotono_core/ObjectManager.h>
#include <kotono_interface/widgets.h>
#include <kotono_core/Game.h>
#include <kotono_core/Scene.h>
#include <kotono_core/SceneObject.h>

WWidget* WTestBox::Build()
{
	ObjectManager.EventSelectedObjectChanged().AddListener(this, &WTestBox::Refresh);

    return new WBox({
        .size = { 100.0f, 100.0f },
        .child = new WStack({
            .children = {
                ObjectManager.GetSelectedObject()
					? new WColor({ UColor::Red() })
                    : new WColor({ UColor::Red().WithValue(0.1f) }),
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
        }),
    });
}

void WTestBox::Cleanup()
{
    ObjectManager.EventSelectedObjectChanged().RemoveListener(this, &WTestBox::Refresh);

	WWidget::Cleanup();
}
