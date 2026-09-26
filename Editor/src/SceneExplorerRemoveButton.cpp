#include "SceneExplorerRemoveButton.h"

#include <kotono_core/Scene.h>
#include <kotono_core/SceneObject.h>
#include <kotono_interface/widgets.h>

WidgetPtr WSceneExplorerRemoveButton::Build()
{   
    return (
		UCreate<WStack>{ "Remove Button Stack" }()
		| (
			UCreate<WButton>{}()
			| Apply(&WButton::SetNormalColor, Colors::Red.WithValue(0.8f))
			| Apply(&WButton::SetDisabledColor, Colors::Red.WithValue(0.2f))
			| Apply(&WButton::SetIsEnabled, [this]() { return GetScene()->GetSelectedObject() != nullptr; })
			| Apply(&WButton::SetOnClicked, [this]() { 
                if (UPtr selectedObject{ GetScene()->GetSelectedObject() })
                {
                    GetScene()->SelectObject(nullptr);
                    selectedObject->Delete();
                } 
            })
		)
		| (
			UCreate<WAlign>{}()
			| Apply(&WAlign::SetAlignment, UAlignment::Center())
			| (
				UCreate<WText>{}()
				| Apply(&WText::SetText, "Remove")
				| Apply(&WText::SetFontSize, glm::vec2{ 14.0f, 18.0f })
			)
		)
	);
}

#include "generated/SceneExplorerRemoveButton.generated.inl"
