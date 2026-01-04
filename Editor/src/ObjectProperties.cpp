#include "ObjectProperties.h"
//#include <kotono_reflection/Reflector.h>
#include <kotono_core/Object.h>
#include <kotono_common/log.h>
#include <kotono_interface/widgets.h>

WObjectProperties::WObjectProperties(const UPtr<KObject>& object) :
	object_(object)
{
}

WWidget* WObjectProperties::Build()
{
 //   if (!object_)
 //   {
 //       return nullptr;
	//}

 //   auto reflectionResult{ Reflector.GetReflectionResult(object_->TypeName()) };

 //   if (!reflectionResult)
 //   {
 //       return nullptr;
 //   }

 //   std::vector<UReflectionResult> hierarchy{};
 //   auto current{ reflectionResult };

 //   while (current)
 //   {
 //       hierarchy.push_back(*current);
 //       const auto& baseName{ current->type.base };
 //       current = baseName ? Reflector.GetReflectionResult(*baseName) : std::nullopt;
 //   }

 //   WidgetVector result{};
 //   for (const auto& item : hierarchy)
 //   {
 //       for (const auto& member : item.members)
 //       {
	//		result.push_back(new WColumn({
 //               .spacing = 4.0f,
 //               .children = {
 //                   new WText({
 //                       .text = member.name,
 //                       .fontSize = { 20.0f, 24.0f },
 //                       .spacing = -6.0f,
 //                   }),
 //                   /*new WPadding({
 //                       .padding = WPadding::Padding::Left(10.0f),
 //                       .child = BuildMemberWidget(member.type),
	//				}),*/
 //                   BuildMemberWidget(member.type),
 //               },
 //           }));
 //       }
 //   }

 //   return new WColumn({
 //       .spacing = 5.0f,
 //       .children = result,
	//});
    return nullptr;
}

void WObjectProperties::Cleanup()
{
	WWidget::Cleanup();
}

WWidget* WObjectProperties::BuildMemberWidget(const std::string& type)
{
	//if (Reflector.IsObjectType(type))
    {
        return nullptr;
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
