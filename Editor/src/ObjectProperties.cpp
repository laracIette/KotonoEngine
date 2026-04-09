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

    WidgetVector result{};
    for (const auto& variable : variables)
    {
        void* variablePtr{ get_member_variable_pointer(object_.Get(), variable.offset) };

        result.push_back(new WColumn({
            .spacing = 4.0f,
            .children = {
                new WText({
                    .text = variable.name,
                    .fontSize = { 20.0f, 24.0f },
                    .spacing = -6.0f,
                }),
                /*new WPadding({
                    .padding = WPadding::Padding::Left(10.0f),
                    .child = BuildMemberWidget(variable.type, variablePtr),
        		}),*/
                BuildMemberWidget(variable.type, variablePtr),
            },
        }));
    }

    return new WWrap({
        .child = new WColumn({
            .spacing = 5.0f,
            .children = result,
        }),
    });
}

void WObjectProperties::Cleanup()
{
	Base::Cleanup();
}

WidgetPtr WObjectProperties::BuildMemberWidget(const std::string& type, void* variablePtr)
{
	//if (Reflector.IsObjectType(type))
    {
        //return nullptr;
    }
	if (type == "f32")
    {
        return new WValueSliderFloat(static_cast<f32*>(variablePtr));
    }
	if (type == "size")
    {
        auto* sizePtr{ static_cast<size*>(variablePtr) };
        return new WValueBox({
            .valueToString = [sizePtr]() {
                return std::to_string(*sizePtr);
            },
            .stringToValue = [sizePtr](const std::string& value) {
                *sizePtr = from_string<size>(value);
            },
        });
    }
    if (type == "std::string")
    {
        auto* stringPtr{ static_cast<std::string*>(variablePtr) };
        return new WValueBox({
            .valueToString = [stringPtr]() {
                return *stringPtr;
            },
            .stringToValue = [stringPtr](const std::string& value) {
                *stringPtr = value;
            },
        });
    }
    if (type == "glm::vec2")
    {
        return new WText({
            .text = "Vec2 Editor Placeholder",
            .fontSize = { 18.0f, 22.0f },
            .spacing = -5.0f,
        });
    }
    if (type == "glm::vec3")
    {
        return new WText({
            .text = "Vec3 Editor Placeholder",
            .fontSize = { 18.0f, 22.0f },
            .spacing = -5.0f,
        });
    }
    if (type == "glm::vec4")
    {
        return new WText({
            .text = "Vec4 Editor Placeholder",
            .fontSize = { 18.0f, 22.0f },
            .spacing = -5.0f,
        });
    }
    return nullptr;
}

#include "generated/ObjectProperties.generated.inl"
