#include "ObjectProperties.h"

#include "SceneObject/SceneObject.h"
#include "ValueBox/ValueBox.h"
#include "ValueSlider/ValueSlider.h"
#include <glm/gtx/string_cast.hpp>
#include <core_widgets.h>

static auto BuildMemberWidget(std::string_view type, void* variablePtr) -> WidgetPtr
{
    if (type == "f32")
    {
        return WValueSlider::FromPointer(static_cast<f32*>(variablePtr));
    }
    if (type == "size")
    {
        return WValueSlider::FromPointer(static_cast<size*>(variablePtr));
    }
    if (type == "std::string")
    {
        return WValueBox::FromPointer(static_cast<std::string*>(variablePtr));
    }
    if (type == "UGuid")
    {
        return WValueBox::FromPointer(static_cast<UGuid*>(variablePtr));
    }
    return (
        UCreate<WText>{}()
        | Apply(&WText::SetText, "Unhandled type")
    );
}

static void ResetValue(std::string_view type, void* variablePtr)
{
    if (type == "f32")
    {
        *static_cast<f32*>(variablePtr) = 0.0f;
    }
    if (type == "size")
    {
        *static_cast<size*>(variablePtr) = 0;
    }
    if (type == "std::string")
    {
        *static_cast<std::string*>(variablePtr) = "";
    }
}

WObjectProperties::WObjectProperties(std::span<UObjectPtr<TSceneObject> const> sceneObjects)
    : sceneObjects_(sceneObjects.begin(), sceneObjects.end())
{
}

auto WObjectProperties::Build() -> WidgetPtr
{
    if (sceneObjects_.empty())
    {
        return (
            UCreate<WText>{}()
            | Apply(&WText::SetText, "No object selected.")
        );
    }

    //std::vector<UVariableInfo> variables{};
    //for (auto const& sceneObject : sceneObjects_)
    //{
    //    for (auto const& variable : sceneObject->GetMemberVariables())
    //    {
    //        if (std::ranges::all_of(sceneObjects_, [variable](UObjectPtr<TSceneObject> const& otherSceneObject) {
    //            return std::ranges::contains(otherSceneObject->GetMemberVariables(), variable);
    //        }))
    //        {
    //            variables.push_back(variable);
    //        }
    //    }
    //}

    UObjectPtr const sceneObject{ sceneObjects_.back() };
    Check(Throw, sceneObject, "sceneObject is null");

    return (
        UCreate<WColumn>{}()
        | Apply(&WColumn::SetSpacing, 5.0f)
        | (
            sceneObject->GetMemberVariables()
            | std::views::transform([sceneObject](UVariableInfo const& variable) {
                void* const variablePtr{ sceneObject->GetMemberVariablePointer(variable.offset) };
                return (
                    UCreate<WWrap>{}()
                    | Apply(&WWrap::SetAxis, EAxis::Vertical)
                    | (
                        UCreate<WRow>{}()
                        | Apply(&WRow::SetSpacing, 4.0f)
                        | (
                            UCreate<WExpanded>{}()
                            | (
                                UCreate<WText>{}()
                                | Apply(&WText::SetText, variable.name)
                            )
                        )
                        | (
                            UCreate<WExpanded>{}()
                            | Apply(&WWidget::SetExpandWeight, glm::vec2{ 0.67f, 1.0f })
                            | (
                                BuildMemberWidget(variable.type, variablePtr)
                            )
                        )
                        | (
                            UCreate<WAlign>{}()
                            | Apply(&WAlign::SetAlignment, UAlignment::Center())
                            | Apply(&WAlign::SetExpandWeight, glm::vec2{ 0.1f, 1.0f })
                            | (
                                UCreate<WButton>{}()
                                | Apply(&WButton::SetOnClicked, [variable, variablePtr](EModifier) { ResetValue(variable.type, variablePtr); })
                            )
                        )
                    )
                );
            })
        )
    );
}

#include "ObjectProperties.generated.inl"
