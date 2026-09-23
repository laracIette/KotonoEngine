#include "ObjectProperties.h"

#include "ValueBox.h"
#include "ValueSliderFloat.h"
#include <glm/gtx/string_cast.hpp>
#include <kotono_common/conversion_utils.h>
#include <kotono_core/Object.h>
#include <kotono_interface/widgets.h>

static void* get_member_variable_pointer(void* object, size offset) noexcept
{
    return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(object) + offset);
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
            | Apply(&WText::SetText, "No object selected")
            | Apply(&WText::SetFontSize, glm::vec2{ 16.0f, 20.0f })
        );
    }

    return (
        UCreate<WWrap>{}()
        | (
            UCreate<WColumn>{}()
            | Apply(&WColumn::SetSpacing, 5.0f)
            | (
                object_->GetMemberVariables()
                | std::views::transform([this](UVariableInfo const& variable) {
                    void* const variablePtr{ get_member_variable_pointer(object_.Get(), variable.offset) };
                    return (
                        UCreate<WColumn>{}()
                        | Apply(&WColumn::SetSpacing, 4.0f)
                        | (
                            UCreate<WText>{}()
                            | Apply(&WText::SetText, variable.name)
                        )
                        | (
                            BuildMemberWidget(variable.type, variablePtr)
                        )
                    );
                })
            )
        )
    );
}

WidgetPtr WObjectProperties::BuildMemberWidget(std::string_view type, void* variablePtr)
{
	//if (Reflector.IsObjectType(type))
    {
        //return nullptr;
    }
	if (type == "f32")
    {
        auto* const floatPtr{ static_cast<f32*>(variablePtr) };
        return (
            UCreate<WValueSliderFloat>{}()
            | Apply(&WValueSliderFloat::SetValueToString, [floatPtr]() { return std::format("{0}", *floatPtr); })
            | Apply(&WValueSliderFloat::SetStringToValue, [floatPtr](std::string_view value) { *floatPtr = from_string<f32>(value); })
        );
    }
	if (type == "size")
    {
        auto* const sizePtr{ static_cast<size*>(variablePtr) };
        return (
            UCreate<WValueBox>{}()
            | Apply(&WValueBox::SetValueToString, [sizePtr]() { return std::format("{0}", *sizePtr); })
            | Apply(&WValueBox::SetStringToValue, [sizePtr](std::string_view value) { *sizePtr = from_string<size>(value); })
        );
    }
    if (type == "std::string")
    {
        auto* const stringPtr{ static_cast<std::string*>(variablePtr) };
        return (
            UCreate<WValueBox>{}()
            | Apply(&WValueBox::SetValueToString, [stringPtr]() { return *stringPtr; })
            | Apply(&WValueBox::SetStringToValue, [stringPtr](std::string_view value) { *stringPtr = value; })
        );
    }
    if (type == "glm::vec2")
    {
        return (
            UCreate<WText>{}()
            | Apply(&WText::SetText, "Vec2 Editor Placeholder")
            | Apply(&WText::SetFontSize, glm::vec2{ 18.0f, 22.0f })
        );
    }
    if (type == "glm::vec3")
    {
        return (
            UCreate<WText>{}()
            | Apply(&WText::SetText, "Vec3 Editor Placeholder")
            | Apply(&WText::SetFontSize, glm::vec2{ 18.0f, 22.0f })
        );
    }
    if (type == "glm::vec4")
    {
        return (
            UCreate<WText>{}()
            | Apply(&WText::SetText, "Vec4 Editor Placeholder")
            | Apply(&WText::SetFontSize, glm::vec2{ 18.0f, 22.0f })
        );
    }
    return nullptr;
}

#include "generated/ObjectProperties.generated.inl"
