#include "SceneExplorerRemoveButton.h"
#include <kotono_interface/widgets.h>
#include <kotono_core/Game.h>
#include <kotono_core/Scene.h>
#include <kotono_core/SceneObject.h>
#include <kotono_object/Object.h>
#include <kotono_object/ObjectManager.h>

WidgetPtr WSceneExplorerRemoveButton::Build()
{
    UPtr color{ Create<WColor>{}() };
    color->SetColor(ObjectManager.GetSelectedObject()
        ? Colors::Red 
        : Colors::Red.WithValue(0.1f)
    );

    UPtr button{ Create<WButton>{}() };
    button->SetOnDown([this]() {
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
    });

    UPtr stack{ Create<WStack>{}() };
    stack->SetChildren({ color, button });

    return stack;
}

void WSceneExplorerRemoveButton::Display(UWidgetDisplaySettings displaySettings)
{
    Base::Display(displaySettings);

    ObjectManager.EventSelectedObjectChanged().AddListener(this, &WSceneExplorerRemoveButton::Refresh);
}

void WSceneExplorerRemoveButton::Remove()
{
    Base::Remove();

    ObjectManager.EventSelectedObjectChanged().RemoveListener(this, &WSceneExplorerRemoveButton::Refresh);
}

#include "generated/SceneExplorerRemoveButton.generated.inl"
