#include "ObjectProperties.h"
#include "ValueBox.h"
#include "ValueSliderFloat.h"
#include <kotono_common/conversion_utils.h>
#include <kotono_common/log.h>
#include <kotono_interface/widgets.h>
#include <kotono_object/Object.h>
#include <glm/gtx/string_cast.hpp>

static void* get_member_variable_pointer(void* object, const size offset) noexcept
{
    return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(object) + offset);
}

WObjectProperties::WObjectProperties(const UPtr<KObject>& object) 
    : object_(object)
{
}

WidgetPtr WObjectProperties::Build()
{
    if (!object_)
    {
        UPtr text{ UCreate<WText>{}() };
        text->SetText("No object selected");
        text->SetFontSize({ 16.0f, 20.0f });
        return text;
    }

    const auto variables{ object_->GetMemberVariables() };

    std::vector<UWidgetTree*> properties{};
    for (const auto& variable : variables)
    {
        void* variablePtr{ get_member_variable_pointer(object_.Get(), variable.offset) };

        UPtr propertyColumn{ UCreate<WColumn>{}() };
        propertyColumn->SetSpacing(4.0f);

        UPtr propertyText{ UCreate<WText>{}() };
        propertyText->SetText(variable.name);
        propertyText->SetFontSize({ 20.0f, 24.0f });
        propertyText->SetSpacing(-6.0f);

        properties.push_back(new UChildrenOwnerTree(propertyColumn, {
            new UWidgetTreeLeaf(propertyText),
            new UWidgetTreeLeaf(BuildMemberWidget(variable.type, variablePtr)),
        }));
    }

    UPtr column{ UCreate<WColumn>{}() };
    column->SetSpacing(5.0f);

    const auto widgetTree{ UChildOwnerTree{ UCreate<WWrap>{}(),
        new UChildrenOwnerTree{ column, properties }
    } };
    widgetTree.Link();

    return widgetTree.Widget();
}

WidgetPtr WObjectProperties::BuildMemberWidget(const std::string& type, void* variablePtr)
{
	//if (Reflector.IsObjectType(type))
    {
        //return nullptr;
    }
	if (type == "f32")
    {
        auto* floatPtr{ static_cast<f32*>(variablePtr) };

        UPtr valueSlider{ UCreate<WValueSliderFloat>{}() };
        valueSlider->SetValueToString([floatPtr]() {
            return std::format("{0}", *floatPtr);
        });
        valueSlider->SetStringToValue([floatPtr](const std::string& value) {
            *floatPtr = from_string<f32>(value);
        });

        return valueSlider;
    }
	if (type == "size")
    {
        auto* sizePtr{ static_cast<size*>(variablePtr) };

        UPtr valueBox{ UCreate<WValueBox>{}() };
        valueBox->SetValueToString([sizePtr]() { 
            return std::format("{0}", *sizePtr);
        });
        valueBox->SetStringToValue([sizePtr](const std::string& value) { 
            *sizePtr = from_string<size>(value); 
        });

        return valueBox;
    }
    if (type == "std::string")
    {
        auto* stringPtr{ static_cast<std::string*>(variablePtr) };

        UPtr valueBox{ UCreate<WValueBox>{}() };
        valueBox->SetValueToString([stringPtr]() {
            return *stringPtr;
        });
        valueBox->SetStringToValue([stringPtr](const std::string& value) {
            *stringPtr = value;
        });

        return valueBox;
    }
    if (type == "glm::vec2")
    {
        UPtr text{ UCreate<WText>{}() };
        text->SetText("Vec2 Editor Placeholder");
        text->SetFontSize({ 18.0f, 22.0f });
        text->SetSpacing(-5.0f);
        return text;
    }
    if (type == "glm::vec3")
    {
        UPtr text{ UCreate<WText>{}() };
        text->SetText("Vec3 Editor Placeholder");
        text->SetFontSize({ 18.0f, 22.0f });
        text->SetSpacing(-5.0f);
        return text;
    }
    if (type == "glm::vec4")
    {
        UPtr text{ UCreate<WText>{}() };
        text->SetText("Vec4 Editor Placeholder");
        text->SetFontSize({ 18.0f, 22.0f });
        text->SetSpacing(-5.0f);
        return text;
    }
    return nullptr;
}

#include "generated/ObjectProperties.generated.inl"
