#include "ObjectProperties.h"
#include <kotono_core/Object.h>
#include <kotono_common/log.h>
#include <kotono_interface/widgets.h>

WObjectProperties::WObjectProperties(const UPtr<KObject>& object) :
	object_(object)
{
}

WWidget* WObjectProperties::Build()
{
    const auto variables{ object_->GetMemberVariables() };
    WidgetVector result{};
    for (const auto& variable : variables)
    {
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
                    .child = BuildMemberWidget(member.type),
        		}),*/
                BuildMemberWidget(variable.type),
            },
        }));
    }

    return new WColumn({
        .spacing = 5.0f,
        .children = result,
    });
}

void WObjectProperties::Cleanup()
{
	WWidget::Cleanup();
}

WWidget* WObjectProperties::BuildMemberWidget(const std::string& type)
{
	//if (Reflector.IsObjectType(type))
    {
        //return nullptr;
    }
	if (type == "float")
    {
        return new WText({
            .text = "Float Editor Placeholder",
            .fontSize = { 18.0f, 22.0f },
            .spacing = -5.0f,
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
	if (type == "std::string")
    {
        return new WText({
            .text = "String Editor Placeholder",
            .fontSize = { 18.0f, 22.0f },
            .spacing = -5.0f,
        });
    }
    return nullptr;
}
