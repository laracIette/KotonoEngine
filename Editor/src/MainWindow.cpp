#include "MainWindow.h"
#include "AssetExplorer.h"
#include "DrawTimeText.h"
#include "GameStateButton.h"
#include "PropertiesWindow.h"
#include "SceneExplorer.h"
#include "UpdateTimeText.h"
#include "VisualizerWindow.h"
#include <kotono_interface/widgets.h>

WidgetPtr WMainWindow::Build()
{	
	UPtr sceneExplorerConstraint{ UCreate<WConstraint>{ "Scene Explorer Constraint" }() };
	sceneExplorerConstraint->SetAxis(EAxis::Horizontal);
	sceneExplorerConstraint->SetSize(300.0f);
	
	//UPtr assetExplorerConstraint{ UCreate<WConstraint>{ "Asset Explorer Constraint" }() };
	//assetExplorerConstraint->SetAxis(EAxis::Vertical);
	//assetExplorerConstraint->SetSize(200.0f);
	
	UPtr leftPanelColumn{ UCreate<WColumn>{ "Left Panel Column" }()};
	leftPanelColumn->SetSpacing(10.0f);
	
	UPtr rightPanelColumn{ UCreate<WColumn>{}() };
	rightPanelColumn->SetSpacing(4.0f);
	
	UPtr rightPanelPadding{ UCreate<WPadding>{}() };
	rightPanelPadding->SetPadding(UPadding::All(8.0f));
	
	UPtr centerRow{ UCreate<WRow>{ "Center Row" }()};
	centerRow->SetSpacing(10.0f);
	
	UPtr mainColumn{ UCreate<WColumn>{ "Main Window Column" }() };
	mainColumn->SetSpacing(5.0f);
	
	UPtr mainPadding{ UCreate<WPadding>{ "Main Window Padding" }() };
	mainPadding->SetPadding(UPadding::All(16.0f));


	const auto widgetTree{ UChildOwnerTree(mainPadding,
		new UChildrenOwnerTree{ mainColumn, {
			new UChildrenOwnerTree{ UCreate<WRow>{ "Top Row" }(), {
				new UWidgetTreeLeaf{ UCreate<WSpacer>{ "Top Row Spacer" }(EAxis::Horizontal) },
				new UChildOwnerTree{ UCreate<WWrap>{ "Times Wrap" }(),
					new UChildrenOwnerTree{ UCreate<WColumn>{ "Times Column" }(), {
						new UWidgetTreeLeaf{ UCreate<WUpdateTimeText>{ "Update Time Text" }() },
						new UWidgetTreeLeaf{ UCreate<WDrawTimeText>{ "Draw Time Text" }() },
					} },
				},
			} },
			new UChildOwnerTree{ UCreate<WWrap>{ "Game State Button Wrap" }(EAxis::Vertical),
				new UChildrenOwnerTree{ UCreate<WRow>{ "Game State Button Row" }(), {
					new UChildOwnerTree{ UCreate<WCenter>{ "Game State Button Center" }(EAxis::Horizontal),
						new UWidgetTreeLeaf{ UCreate<WGameStateButton>{ "Game State Button" }() }
					}
				} }
			},
			new UChildrenOwnerTree{ centerRow, {
				//new UChildOwnerTree{ UCreate<WWrap>{ "Left Panel Wrap" }(EAxis::Horizontal),
					new UChildrenOwnerTree{ leftPanelColumn, {
						new UChildOwnerTree{ sceneExplorerConstraint,
							new UWidgetTreeLeaf{ UCreate<WSceneExplorer>{}() }
						},
						//new UWidgetTreeLeaf{ UCreate<WSpacer>{ "Left Panel Column Spacer" }(EAxis::Vertical) },
						//new UChildOwnerTree{ assetExplorerConstraint,
						//	new UWidgetTreeLeaf{ UCreate<WAssetExplorer>{ "Asset Explorer" }()}
						//},
					} },
				//},
				new UWidgetTreeLeaf{ UCreate<WSpacer>{ "Center Row Spacer" }(EAxis::Horizontal) },
				new UChildOwnerTree{ UCreate<WWrap>{ "Right Panel Wrap" }(EAxis::Horizontal),
					new UChildrenOwnerTree{ UCreate<WStack>{}(), {
						new UWidgetTreeLeaf{ UCreate<WColor>{ "Right Panel Background" }(Colors::Magenta.WithAlpha(0.2f))},
						new UChildOwnerTree{ rightPanelPadding,
							new UChildrenOwnerTree{ rightPanelColumn, {
								new UWidgetTreeLeaf{ UCreate<WVisualizerWindow>{}() },
								new UWidgetTreeLeaf{ UCreate<WPropertiesWindow>{}() },
							} }
						},
					} }
				},
			} },
		} }
	) };
	
	widgetTree.Link();
	
	return widgetTree.Widget();
}

#include "generated/MainWindow.generated.inl"
