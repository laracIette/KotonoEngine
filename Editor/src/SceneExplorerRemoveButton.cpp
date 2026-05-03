#include "SceneExplorerRemoveButton.h"
#include <kotono_interface/widgets.h>
#include <kotono_core/Game.h>
#include <kotono_core/Scene.h>
#include <kotono_core/SceneObject.h>
#include <kotono_object/Object.h>
#include <kotono_object/ObjectManager.h>

WidgetPtr WSceneExplorerRemoveButton::Build()
{
    UPtr color{ UCreate<WColor>{}() };
    color->SetColor(ObjectManager.GetSelectedObject()
        ? Colors::Red 
        : Colors::Red.WithValue(0.1f)
    );

    UPtr button{ UCreate<WButton>{}() };
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

    UPtr stack{ UCreate<WStack>{}() };
    stack->SetChildren({ color, button });

    return stack;
}

void WSceneExplorerRemoveButton::Display(UWidgetDisplaySettings displaySettings)
{
    Base::Display(displaySettings);

    ObjectManager.EventSelectedObjectChanged().AddListener(this, &Self::OnSelectedObjectChanged);
}

void WSceneExplorerRemoveButton::Remove()
{
    Base::Remove();

    ObjectManager.EventSelectedObjectChanged().RemoveListener(this, &Self::OnSelectedObjectChanged);
}

void WSceneExplorerRemoveButton::OnSelectedObjectChanged(const UPtr<KObject> object)
{
    Refresh();
}

#include "generated/SceneExplorerRemoveButton.generated.inl"
