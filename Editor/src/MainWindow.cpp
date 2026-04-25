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
	UPtr timesColumn{ Create<WColumn>{}() };
	timesColumn->SetSpacing(2.0f);
	timesColumn->SetName("Times Column");
	
	UPtr gameStateButton{ Create<WCenter>{}() };
	gameStateButton->SetAxis(EAxis::Horizontal);
	
	UPtr sceneExplorerConstraint{ Create<WConstraint>{}() };
	sceneExplorerConstraint->SetAxis(EAxis::Horizontal);
	sceneExplorerConstraint->SetSize(300.0f);
	
	UPtr assetExplorerConstraint{ Create<WConstraint>{}() };
	assetExplorerConstraint->SetAxis(EAxis::Vertical);
	assetExplorerConstraint->SetSize(200.0f);
	
	UPtr leftPanelColumn{ Create<WColumn>{}() };
	leftPanelColumn->SetSpacing(10.0f);
	
	UPtr rightPanelBg{ Create<WColor>{}() };
	rightPanelBg->SetColor(Colors::Magenta.WithAlpha(0.2f));
	
	UPtr rightPanelColumn{ Create<WColumn>{}() };
	rightPanelColumn->SetSpacing(4.0f);
	
	UPtr rightPanelPadding{ Create<WPadding>{}() };
	rightPanelPadding->SetPadding(UPadding::All(8.0f));
	
	UPtr centerRow{ Create<WRow>{}() };
	centerRow->SetSpacing(10.0f);
	
	UPtr mainColumn{ Create<WColumn>{}() };
	mainColumn->SetSpacing(5.0f);
	mainColumn->SetName("Main Window Column");
	
	UPtr mainPadding{ Create<WPadding>{}() };
	mainPadding->SetPadding(UPadding::All(16.0f));
	mainPadding->SetName("Main Window Padding");


	const auto widgetTree{ UChildOwnerTree(mainPadding,
		new UChildrenOwnerTree(mainColumn, {
			new UChildrenOwnerTree(Create<WRow>{}(), {
				new UWidgetTreeLeaf(Create<WSpacer>{}(EFlex::Horizontal)),
				new UChildrenOwnerTree(timesColumn, {
					new UWidgetTreeLeaf(Create<WUpdateTimeText>{}()),
					new UWidgetTreeLeaf(Create<WDrawTimeText>{}()),
				}),
			}),
			new UChildOwnerTree(Create<WWrap>{}(),
				new UChildOwnerTree(gameStateButton,
					new UChildOwnerTree(Create<WWrap>{}(),
						new UWidgetTreeLeaf(Create<WGameStateButton>{}())
					)
				)
			),
			new UChildOwnerTree(Create<WExpanded>{}(),
				new UChildrenOwnerTree(centerRow, {
					new UChildrenOwnerTree(leftPanelColumn, {
						new UChildOwnerTree(sceneExplorerConstraint,
							new UWidgetTreeLeaf(Create<WSceneExplorer>{}())
						),
						new UChildOwnerTree(assetExplorerConstraint,
							new UWidgetTreeLeaf(Create<WAssetExplorer>{}())
						),
					}),
					new UChildOwnerTree(Create<WWrap>{}(),
						new UChildrenOwnerTree(Create<WStack>{}(), {
							new UWidgetTreeLeaf(rightPanelBg),
							new UChildOwnerTree(rightPanelPadding,
								new UChildrenOwnerTree(rightPanelColumn, {
									new UWidgetTreeLeaf(Create<WVisualizerWindow>{}()),
									new UWidgetTreeLeaf(Create<WPropertiesWindow>{}()),
								})
							),
						})
					),
				})
			),
		})
	) };
	
	widgetTree.Link();
	
	return widgetTree.Widget();
}

#include "generated/MainWindow.generated.inl"
