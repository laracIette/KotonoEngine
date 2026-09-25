#include "VisualizerWindow.h"

#include "VisualizerWindowItem.h"
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

WVisualizerWindow::WVisualizerWindow()
    : sceneVisibility_{ ESceneVisibility::All }
{
}

WidgetPtr WVisualizerWindow::Build()
{
    return (
        UCreate<WBox>{}()
        | Apply(&WBox::SetSize, glm::vec2{ 400.0f, 300.0f })
        | (
            UCreate<WStack>{}()
            | (
                UCreate<WColor>{}()
                | Apply(&WColor::SetColor, Colors::Blue.WithAlpha(0.5f))
            )
            | (
                UCreate<WPadding>{}()
                | Apply(&WPadding::SetPadding, UPadding::All(8.0f))
                | (
                    UCreate<WColumn>{}()
                    | Apply(&WColumn::SetSpacing, 10.0f)
                    | (
                        UCreate<WWrap>{}()
                        | (
                            UCreate<WStack>{}()
                            | (
                                UCreate<WColor>{}()
                                | Apply(&WColor::SetColor, Colors::Black.WithAlpha(0.5f))
                            )
                            | (
                                UCreate<WText>{}()
                                | Apply(&WText::SetText, "Visualizer")
                            )
                        )
                    )
                    | (
                        UCreate<WList>{}()
                        | Apply(&WList::SetSpacing, 5.0f)
                        | (
                            FIELDS 
                            | std::views::transform([this](VisibilityField const& field) {
                                return (
                                    UCreate<WVisualizerWindowItem>{}(field.visibility, field.name)
                                    | Apply(&WVisualizerWindowItem::SetOnVisibilityChanged, [this](ESceneVisibility field, b8 isActive) {
                                        if (isActive)
                                        {
                                            sceneVisibility_ |= field;
                                        }
                                        else
                                        {
                                            sceneVisibility_ &= ~field;
                                        }

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

#include "generated/VisualizerWindow.generated.inl"
