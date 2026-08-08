#include "MainWindow.h"
#include "AssetExplorer.h"
#include "DrawTimeText.h"
#include "GameStateButton.h"
#include "PropertiesWindow.h"
#include "SceneExplorer.h"
#include "UpdateTimeText.h"
#include "ViewController.h"
#include "VisualizerWindow.h"
#include <kotono_graphics/RenderContext.h>
#include <kotono_interface/widgets.h>
#include <kotono_platform/Window.h>

WidgetPtr WMainWindow::Build()
{		
	const auto widgetTree{ UChildOwnerTree{ UCreate<WPadding>{ "Main Window Padding" }(UPadding::All(16.0f)),
		new UChildrenOwnerTree{ UCreate<WColumn>{ "Main Window Column" }(5.0f), {
			new UChildrenOwnerTree{ UCreate<WRow>{ "Top Row" }(), {
				new UWidgetTreeLeaf{ UCreate<WSpacer>{ "Top Row Spacer" }(EAxis::Horizontal) },
				new UChildOwnerTree{ UCreate<WWrap>{ "Times Wrap" }(),
					new UChildrenOwnerTree{ UCreate<WColumn>{ "Times Column" }(), {
						new UWidgetTreeLeaf{ UCreate<WUpdateTimeText>{ "Update Time Text" }() },
						new UWidgetTreeLeaf{ UCreate<WDrawTimeText>{ "Draw Time Text" }() },
					} },
				},
			} },
			new UChildOwnerTree{ UCreate<WWrap>{ "Game State Wrap" }(EAxis::Vertical),
				new UChildrenOwnerTree{ UCreate<WRow>{ "Game State Row" }(), {
					new UChildOwnerTree{ UCreate<WCenter>{ "Game State Center" }(EAxis::Horizontal),
						new UWidgetTreeLeaf{ UCreate<WGameStateButton>{ "Game State Button" }() }
					},
				} }
			},
			new UChildrenOwnerTree{ UCreate<WRow>{ "Center Row" }(10.0f), {
				new UChildOwnerTree{ UCreate<WExpanded>{ "Left Panel Expanded" }(), {
					new UChildrenOwnerTree{ UCreate<WColumn>{ "Left Panel Column" }(10.0f), {
						new UChildOwnerTree{ UCreate<WConstraint>{ "Left Panel Constraint" }(EAxis::Vertical, 500.0f),
							new UChildrenOwnerTree{ UCreate<WRow>{}(), {
								new UChildOwnerTree{ UCreate<WConstraint>{ "Scene Explorer Constraint" }(EAxis::Horizontal, 300.0f),
									new UWidgetTreeLeaf{ UCreate<WSceneExplorer>{ "Scene Explorer" }() }
								},
								new UWidgetTreeLeaf{ UCreate<WViewController>{ "Scene View Controller" }() },
							} }
						},
						new UWidgetTreeLeaf{ UCreate<WAssetExplorer>{ "Asset Explorer" }() },
					} },
				} },

				new UChildOwnerTree{ UCreate<WWrap>{ "Right Panel Wrap" }(EAxis::Horizontal),
					new UChildrenOwnerTree{ UCreate<WStack>{ "Right Panel Stack" }(), {
						new UWidgetTreeLeaf{ UCreate<WColor>{ "Right Panel Background" }(Colors::White.WithValue(0.5f).WithAlpha(0.4f))},
						new UChildOwnerTree{ UCreate<WPadding>{ "Right Panel Padding" }(UPadding::All(8.0f)),
							new UChildrenOwnerTree{ UCreate<WColumn>{ "Right Panel Column" }(4.0f), {
								//new UWidgetTreeLeaf{ UCreate<WVisualizerWindow>{ "Visualizer Window" }() },
								new UWidgetTreeLeaf{ UCreate<WPropertiesWindow>{ "Properties Window" }() },
							} }
						},
					} }
				},
			} },
		} }
	} };
	widgetTree.Link();
	
	return widgetTree.Widget();
}

void WMainWindow::BeginDraw()
{
	Display({
		.position = { 0.0f, 0.0f },
		.bounds = static_cast<glm::vec2>(GetRenderContext()->GetViewport().GetExtent()),
		.layer = 0,
		.scissor = { { 0, 0 }, GetRenderContext()->GetViewport().GetExtent() },
		});
}

void WMainWindow::EndDraw()
{
	Remove();
}

void WMainWindow::Display(UWidgetDisplaySettings displaySettings)
{
	Base::Display(displaySettings);

	Window.GetEventWindowResized().AddListener(this, &WMainWindow::OnWindowResized);
}

void WMainWindow::Remove()
{
	Base::Remove(); 
	
	Window.GetEventWindowResized().RemoveListener(this, &WMainWindow::OnWindowResized);
}

void WMainWindow::OnWindowResized(const glm::uvec2 extent)
{
	EndDraw();
	BeginDraw();
}

#include "generated/MainWindow.generated.inl"
