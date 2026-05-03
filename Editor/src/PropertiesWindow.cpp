#include "PropertiesWindow.h"
#include "ObjectProperties.h"
#include <kotono_common/log.h>
#include <kotono_core/InterfaceComponent.h>
#include <kotono_core/InterfaceObject.h>
#include <kotono_core/SceneComponent.h>
#include <kotono_core/SceneObject.h>
#include <kotono_input/Mouse.h>
#include <kotono_interface/widgets.h>
#include <kotono_object/ObjectManager.h>
#include <kotono_platform/glm_utils.h>

WidgetPtr WPropertiesWindow::Build()
{
    UPtr selectedObject{ TryCast<TSceneObject>(ObjectManager.GetSelectedObject()) };

    if (!selectedObject)
    {
        UPtr text{ UCreate<WText>{}() };
        text->SetText("No scene object selected");
        text->SetFontSize({ 16.0f, 20.0f });
		return text;
    }

    UPtr sliderColumn{ UCreate<WColumn>{}() };
    sliderColumn->SetChildren({
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
    });
    sliderColumn->SetSpacing(10.0f);


    UPtr propertiesText{ UCreate<WText>{}() };
    propertiesText->SetText("Properties");
    propertiesText->SetSpacing(-20.0f);

    UPtr propertiesTextBg{ UCreate<WColor>{}() };
    propertiesTextBg->SetColor(Colors::Black.WithAlpha(0.5f));

    UPtr propertiesTextStack{ UCreate<WStack>{}() };
    propertiesTextStack->SetChildren({ propertiesTextBg, propertiesText });

    UPtr propertiesTextWrap{ UCreate<WWrap>{}() };
    propertiesTextWrap->SetChild(propertiesTextStack);


    UPtr mainList{ UCreate<WList>{}() };
    mainList->SetChildren({ propertiesTextWrap, UCreate<WObjectProperties>{}(selectedObject), sliderColumn});
    mainList->SetSpacing(10.0f);

    UPtr mainListPadding{ UCreate<WPadding>{}() };
    mainListPadding->SetChild(mainList);
    mainListPadding->SetPadding(UPadding::All(8.0f));

    UPtr mainListBg{ UCreate<WColor>{}() };
    mainListBg->SetColor(Colors::Blue.WithAlpha(0.5f));

    UPtr mainStack{ UCreate<WStack>{}() };
    mainStack->SetChildren({ mainListBg, mainListPadding });

    UPtr mainWrap{ UCreate<WWrap>{}() };
    mainWrap->SetChild(mainStack);

    return mainWrap;
}

void WPropertiesWindow::Display(UWidgetDisplaySettings displaySettings)
{
    Base::Display(displaySettings);

    ObjectManager.EventSelectedObjectChanged().AddListener(this, &Self::OnSelectedObjectChanged);
}

void WPropertiesWindow::Remove()
{
    Base::Remove();

    ObjectManager.EventSelectedObjectChanged().RemoveListener(this, &Self::OnSelectedObjectChanged);
}

void WPropertiesWindow::OnSelectedObjectChanged(const UPtr<KObject> object)
{
    Refresh();
}

WidgetPtr WPropertiesWindow::Slider(const std::string& label, const ValueChangedFunction& function)
{
    UPtr button{ UCreate<WButton>{}() };
    button->SetOnDown([function]() {
        const float delta{ Mouse.CursorPositionDelta().x };
        if (delta != 0.0f)
        {
            function(delta);
        }
    });

    UPtr buttonText{ UCreate<WText>{}() };
    buttonText->SetText(label);
    buttonText->SetFontSize({ 20.0f, 25.0f });
    buttonText->SetSpacing(-8.0f);

    UPtr buttonBg{ UCreate<WColor>{}() };
    buttonBg->SetColor(Colors::White.WithValue(0.5f));


    UPtr buttonStack{ UCreate<WStack>{}() };
    buttonStack->SetChildren({ buttonBg, buttonText, button });

    UPtr buttonWrap{ UCreate<WWrap>{}() };
    buttonWrap->SetChild(buttonStack);
    buttonWrap->SetAxis(EAxis::Vertical);

    return buttonWrap;
}

#include "generated/PropertiesWindow.generated.inl"
