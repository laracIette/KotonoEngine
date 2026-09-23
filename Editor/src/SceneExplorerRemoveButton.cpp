#include "SceneExplorerRemoveButton.h"

#include <kotono_core/Scene.h>
#include <kotono_core/SceneObject.h>
#include <kotono_interface/widgets.h>

WidgetPtr WSceneExplorerRemoveButton::Build()
{   
    return (
		UCreate<WStack>{ "Remove Button Stack" }()
		| (
			button_ = UCreate<WButton>{}()
			| Apply(&WButton::SetNormalColor, Colors::Red.WithValue(0.8f))
			| Apply(&WButton::SetDisabledColor, Colors::Red.WithValue(0.2f))
			| Apply(&WButton::SetIsEnabled, false)
			| Apply(&WButton::SetOnClicked, [this]() { 
                if (UPtr selectedObject{ GetScene()->GetSelectedObject() })
                {
                    GetScene()->Remove(selectedObject);
                    GetScene()->SelectObject(nullptr);
                    selectedObject->Delete();
                } 
            })
		)
		| (
			UCreate<WCenter>{}()
			| (
				UCreate<WText>{}()
				| Apply(&WText::SetText, "Remove")
				| Apply(&WText::SetFontSize, glm::vec2{ 14.0f, 18.0f })
			)
		)
	);
}

void WSceneExplorerRemoveButton::Display(UWidgetDisplaySettings const& displaySettings)
{
    Base::Display(displaySettings);

    GetScene()->GetEventSelectedObjectChanged().AddListener(this, &Self::OnSelectedObjectChanged);
}

void WSceneExplorerRemoveButton::Remove()
{
    Base::Remove();

    GetScene()->GetEventSelectedObjectChanged().RemoveListener(this, &Self::OnSelectedObjectChanged);
}

void WSceneExplorerRemoveButton::OnSelectedObjectChanged(UPtr<TSceneObject> const& sceneObject) const
{
    button_->SetIsEnabled(sceneObject != nullptr);
}

#include "generated/SceneExplorerRemoveButton.generated.inl"
