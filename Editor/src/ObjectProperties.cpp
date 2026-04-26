#include "ObjectProperties.h"
#include "ValueBox.h"
#include "ValueSliderFloat.h"
#include <kotono_common/log.h>
#include <kotono_interface/widgets.h>
#include <kotono_object/Object.h>
#include <glm/gtx/string_cast.hpp>

template<typename T>
static T from_string(const std::string& string, T defaultValue = T{})
{
    T result;
    auto [ptr, ec] { std::from_chars(string.data(), string.data() + string.size(), result) };
    return (ec == std::errc()) ? result : defaultValue;
}

static void* get_member_variable_pointer(void* object, const size offset) noexcept
{
    return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(object) + offset);
}

WObjectProperties::WObjectProperties(const UPtr<KObject>& object) :
	object_(object)
{
}

WidgetPtr WObjectProperties::Build()
{
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

    UPtr wrap{ UCreate<WWrap>{}() };

    UPtr column{ UCreate<WColumn>{}() };
    column->SetSpacing(5.0f);

    UChildOwnerTree(wrap, 
        new UChildrenOwnerTree(column, properties)
    ).Link();

    return wrap;
}

WidgetPtr WObjectProperties::BuildMemberWidget(const std::string& type, void* variablePtr)
{
	//if (Reflector.IsObjectType(type))
    {
        //return nullptr;
    }
	if (type == "f32")
    {
        return UCreate<WValueSliderFloat>{}(static_cast<f32*>(variablePtr));
    }
	if (type == "size")
    {
        auto* sizePtr{ static_cast<size*>(variablePtr) };

        UPtr valueBox{ UCreate<WValueBox>{}() };
        valueBox->SetValueToString([sizePtr]() { 
            return std::to_string(*sizePtr); 
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
