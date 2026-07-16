#include "MainWindow.h"
#include "AssetExplorer.h"
#include "DrawTimeText.h"
#include "GameStateButton.h"
#include "PropertiesWindow.h"
#include "SceneExplorer.h"
#include "UpdateTimeText.h"
#include "VisualizerWindow.h"
#include <kotono_interface/widgets.h>
#include <kotono_platform/WindowViewport.h>

void WMainWindow::BeginDraw()
{
	Display({
		.position = { 0.0f, 0.0f },
		.bounds = static_cast<glm::vec2>(WindowViewport.GetExtent()),
		.layer = 0,
		.scissor = { { 0, 0 }, WindowViewport.GetExtent()},
	});
}

void WMainWindow::EndDraw()
{
	Remove();
}

WidgetPtr WMainWindow::Build()
{	
	UPtr sceneExplorerConstraint{ UCreate<WConstraint>{ "Scene Explorer Constraint" }() };
	sceneExplorerConstraint->SetAxis(EAxis::Horizontal);
	sceneExplorerConstraint->SetSize(300.0f);
	
	UPtr assetExplorerConstraint{ UCreate<WConstraint>{ "Asset Explorer Constraint" }() };
	assetExplorerConstraint->SetAxis(EAxis::Vertical);
	assetExplorerConstraint->SetSize(200.0f);


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
						new UChildOwnerTree{ sceneExplorerConstraint,
							new UWidgetTreeLeaf{ UCreate<WSceneExplorer>{ "Scene Explorer" }()}
						},
						new UChildOwnerTree{ assetExplorerConstraint,
							new UWidgetTreeLeaf{ UCreate<WAssetExplorer>{ "Asset Explorer" }() }
						},
					} }
				} },

				new UChildOwnerTree{ UCreate<WWrap>{ "Right Panel Wrap" }(EAxis::Horizontal),
					new UChildrenOwnerTree{ UCreate<WStack>{ "Right Panel Stack" }(), {
						new UWidgetTreeLeaf{ UCreate<WColor>{ "Right Panel Background" }(Colors::Magenta.WithAlpha(0.9f))},
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

#include "generated/MainWindow.generated.inl"
