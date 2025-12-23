#include "TestBox.h"
#include <kotono_core/Object.h>
#include <kotono_core/ObjectManager.h>
#include <kotono_interface/widgets.h>
#include <kotono_core/Game.h>
#include <kotono_core/Scene.h>
#include <kotono_core/SceneObject.h>

WWidget* WTestBox::Build()
{
	ObjectManager.EventSelectedObjectChanged().AddListener(KtDelegate(this, &WTestBox::Refresh));

    return new WBox({
        .size = { 100.0f, 100.0f },
        .child = new WStack({
            .children = {
                ObjectManager.GetSelectedObject()
					? new WColor({ KtColor::Red() })
                    : new WColor({ KtColor::Red().WithValue(0.1f) }),
                new WButton({
                    .onDown = [this]() {
                        if (UPtr selectedObject{ ObjectManager.GetSelectedObject() })
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
    ObjectManager.EventSelectedObjectChanged().RemoveListener(KtDelegate(this, &WTestBox::Refresh));

	WWidget::Cleanup();
}
