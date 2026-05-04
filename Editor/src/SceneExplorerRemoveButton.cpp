#include "SceneExplorerRemoveButton.h"
#include <kotono_interface/widgets.h>
#include <kotono_core/Game.h>
#include <kotono_core/Scene.h>
#include <kotono_core/SceneObject.h>
#include <kotono_object/Object.h>
#include <kotono_object/ObjectManager.h>

WidgetPtr WSceneExplorerRemoveButton::Build()
{
    bg_ = UCreate<WColor>{}();
    bg_->SetColor(ObjectManager.GetSelectedObject()
        ? Colors::Red 
        : Colors::Red.WithValue(0.1f)
    );

    button_ = UCreate<WButton>{}();
    button_->SetIsEnabled(false);
    button_->SetOnClicked([]() {
        if (UPtr selectedObject{ TryCast<TSceneObject>(ObjectManager.GetSelectedObject()) })
        {
            if (UPtr scene{ Game.GetOpenedScene() })
            {
                scene->Remove(selectedObject);
            }
            ObjectManager.SetSelectedObject({});
            selectedObject->Delete();
        }
    });

    UPtr stack{ UCreate<WStack>{}() };
    stack->SetChildren({ bg_, button_ });

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

void WSceneExplorerRemoveButton::OnSelectedObjectChanged(const UPtr<KObject> object) const
{
    bg_->SetColor(object
        ? Colors::Red
        : Colors::Red.WithValue(0.1f)
    );
    button_->SetIsEnabled(object);
}

#include "generated/SceneExplorerRemoveButton.generated.inl"
