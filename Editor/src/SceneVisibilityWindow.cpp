#include "SceneVisibilityWindow.h"

#include "SceneVisibilityField.h"
#include <array>
#include <kotono_common/enum_utils.h>
#include <kotono_interface/widgets.h>

struct VisibilityField
{
    ESceneVisibility visibility;
    std::string_view name;
};

static constexpr std::array FIELDS{ 
    VisibilityField{ ESceneVisibility::All, "All" },
    VisibilityField{ ESceneVisibility::Mesh, "Mesh" },
    VisibilityField{ ESceneVisibility::Bounds, "Bounds" },
    VisibilityField{ ESceneVisibility::Collider, "Collider" },
    VisibilityField{ ESceneVisibility::Wireframe, "Wireframe" },
    VisibilityField{ ESceneVisibility::DirectionalLight, "Directional Light" },
    VisibilityField{ ESceneVisibility::PointLight, "Point Light" },
};

WSceneVisibilityWindow::WSceneVisibilityWindow(ESceneVisibility sceneVisibility)
    : sceneVisibility_{ sceneVisibility }
{
}

WSceneVisibilityWindow::WSceneVisibilityWindow()
    : Self(ESceneVisibility::All)
{
}

WidgetPtr WSceneVisibilityWindow::Build()
{
    return (
        UCreate<WWrap>{}()
        | (
            UCreate<WStack>{}()
            | (
                UCreate<WColor>{}()
                | Apply(&WColor::SetColor, Colors::Blue.WithAlpha(0.2f))
            )
            | (
                UCreate<WPadding>{}()
                | Apply(&WPadding::SetPadding, UPadding::All(8.0f))
                | (
                    UCreate<WColumn>{}()
                    | Apply(&WColumn::SetSpacing, 10.0f)
                    | (
                        UCreate<WText>{}()
                        | Apply(&WText::SetText, "Scene Visibility")
                    )
                    | (
                        UCreate<WList>{}()
                        | Apply(&WList::SetSpacing, 5.0f)
                        | (
                            FIELDS 
                            | std::views::transform([this](VisibilityField const& field) {
                                return (
                                    UCreate<WSceneVisibilityField>{}(field.visibility, field.name, has_flag(sceneVisibility_, field.visibility))
                                    | Apply(&WSceneVisibilityField::SetOnVisibilityChanged, [this](ESceneVisibility field, b8 isActive) {
                                        sceneVisibility_ = isActive
                                            ? sceneVisibility_ | field
                                            : sceneVisibility_ & ~field;

                                        if (onSceneVisibilityChanged_)
                                        {
                                            onSceneVisibilityChanged_(sceneVisibility_);
                                        }
                                    })
                                );
                            })
                        )
                    )
                )
            )
        )
    );
}

#include "generated/SceneVisibilityWindow.generated.inl"
