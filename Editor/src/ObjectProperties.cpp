#include "ObjectProperties.h"

#include "ValueBox.h"
#include "ValueSlider.h"
#include <glm/gtx/string_cast.hpp>
#include <kotono_core/Object.h>
#include <kotono_interface/widgets.h>

static auto buildMemberWidget(std::string_view type, void* variablePtr) -> WidgetPtr
{
    //if (Reflector.IsObjectType(type))
    {
        //return nullptr;
    }
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
    if (type == "glm::vec2")
    {
        return (
            UCreate<WText>{}()
            | Apply(&WText::SetText, "Vec2 Editor Placeholder")
        );
    }
    if (type == "glm::vec3")
    {
        return (
            UCreate<WText>{}()
            | Apply(&WText::SetText, "Vec3 Editor Placeholder")
        );
    }
    if (type == "glm::vec4")
    {
        return (
            UCreate<WText>{}()
            | Apply(&WText::SetText, "Vec4 Editor Placeholder")
        );
    }
    return (
        UCreate<WText>{}()
        | Apply(&WText::SetText, "Unhandled type")
    );
}

static void resetValue(std::string_view type, void* variablePtr)
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

WObjectProperties::WObjectProperties(ObjectPtr const& object)
    : object_{ object }
{
}

WidgetPtr WObjectProperties::Build()
{
    if (!object_)
    {
        return (
            UCreate<WText>{}()
            | Apply(&WText::SetText, "No object selected.")
        );
    }

    return (
        UCreate<WColumn>{}()
        | Apply(&WColumn::SetSpacing, 5.0f)
        | (
            object_->GetMemberVariables()
            | std::views::transform([this](UVariableInfo const& variable) {
                void* const variablePtr{ object_->GetMemberVariablePointer(variable.offset) };
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
                                buildMemberWidget(variable.type, variablePtr)
                            )
                        )
                        | (
                            UCreate<WAlign>{}()
                            | Apply(&WAlign::SetAlignment, UAlignment::Center())
                            | Apply(&WAlign::SetExpandWeight, glm::vec2{ 0.1f, 1.0f })
                            | (
                                UCreate<WButton>{}()
                                | Apply(&WButton::SetOnClicked, [variable, variablePtr]() { resetValue(variable.type, variablePtr); })
                            )
                        )
                    )
                );
            })
        )
    );
}

#include "generated/ObjectProperties.generated.inl"
