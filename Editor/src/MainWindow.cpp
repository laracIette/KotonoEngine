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
	UPtr timesColumn{ UCreate<WColumn>{}() };
	timesColumn->SetSpacing(2.0f);
	timesColumn->SetName("Times Column");
	
	UPtr gameStateButton{ UCreate<WCenter>{}() };
	gameStateButton->SetAxis(EAxis::Horizontal);
	
	UPtr sceneExplorerConstraint{ UCreate<WConstraint>{}() };
	sceneExplorerConstraint->SetAxis(EAxis::Horizontal);
	sceneExplorerConstraint->SetSize(300.0f);
	
	UPtr assetExplorerConstraint{ UCreate<WConstraint>{}() };
	assetExplorerConstraint->SetAxis(EAxis::Vertical);
	assetExplorerConstraint->SetSize(200.0f);
	
	UPtr leftPanelColumn{ UCreate<WColumn>{}() };
	leftPanelColumn->SetSpacing(10.0f);
	
	UPtr rightPanelBg{ UCreate<WColor>{}() };
	rightPanelBg->SetColor(Colors::Magenta.WithAlpha(0.2f));
	
	UPtr rightPanelColumn{ UCreate<WColumn>{}() };
	rightPanelColumn->SetSpacing(4.0f);
	
	UPtr rightPanelPadding{ UCreate<WPadding>{}() };
	rightPanelPadding->SetPadding(UPadding::All(8.0f));
	
	UPtr centerRow{ UCreate<WRow>{}() };
	centerRow->SetSpacing(10.0f);
	
	UPtr mainColumn{ UCreate<WColumn>{}() };
	mainColumn->SetSpacing(5.0f);
	mainColumn->SetName("Main Window Column");
	
	UPtr mainPadding{ UCreate<WPadding>{}() };
	mainPadding->SetPadding(UPadding::All(16.0f));
	mainPadding->SetName("Main Window Padding");


	const auto widgetTree{ UChildOwnerTree(mainPadding,
		new UChildrenOwnerTree(mainColumn, {
			new UChildrenOwnerTree(UCreate<WRow>{}(), {
				new UWidgetTreeLeaf(UCreate<WSpacer>{}(EFlex::Horizontal)),
				new UChildrenOwnerTree(timesColumn, {
					new UWidgetTreeLeaf(UCreate<WUpdateTimeText>{}()),
					new UWidgetTreeLeaf(UCreate<WDrawTimeText>{}()),
				}),
			}),
			new UChildOwnerTree(UCreate<WWrap>{}(),
				new UChildOwnerTree(gameStateButton,
					new UChildOwnerTree(UCreate<WWrap>{}(),
						new UWidgetTreeLeaf(UCreate<WGameStateButton>{}())
					)
				)
			),
			new UChildOwnerTree(UCreate<WExpanded>{}(),
				new UChildrenOwnerTree(centerRow, {
					new UChildrenOwnerTree(leftPanelColumn, {
						new UChildOwnerTree(sceneExplorerConstraint,
							new UWidgetTreeLeaf(UCreate<WSceneExplorer>{}())
						),
						new UChildOwnerTree(assetExplorerConstraint,
							new UWidgetTreeLeaf(UCreate<WAssetExplorer>{}())
						),
					}),
					new UChildOwnerTree(UCreate<WWrap>{}(),
						new UChildrenOwnerTree(UCreate<WStack>{}(), {
							new UWidgetTreeLeaf(rightPanelBg),
							new UChildOwnerTree(rightPanelPadding,
								new UChildrenOwnerTree(rightPanelColumn, {
									new UWidgetTreeLeaf(UCreate<WVisualizerWindow>{}()),
									new UWidgetTreeLeaf(UCreate<WPropertiesWindow>{}()),
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
