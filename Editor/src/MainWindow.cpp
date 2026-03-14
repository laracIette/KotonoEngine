#include "MainWindow.h"
#include "AssetExplorer.h"
#include "DrawTimeText.h"
#include "GameStateButton.h"
#include "PropertiesWindow.h"
#include "SceneExplorer.h"
#include "TestBox.h"
#include "UpdateTimeText.h"
#include "VisualizerWindow.h"
#include <kotono_core/Timer.h>
#include <kotono_interface/widgets.h>

WidgetPtr WMainWindow::Build()
{
	return new WPadding({
		.padding = WPadding::Padding::All(16.0f),
		.child = new WColumn({
			.spacing = 5.0f,
			.children = {
				new WRow({
					.spacing = 4.0f,
					.children = {
						new WSpacer({}),
						new WColumn({
							.spacing = 2.0f,
							.children = {
								new WUpdateTimeText(),
								new WDrawTimeText(),
							},
						}),
					},
				}),
				new WRow({
					.children = {
						new WSpacer({}),
						new WGameStateButton({}),
						new WSpacer({}),
					},
				}),
				new WExpanded({
					.child = new WRow({
						.spacing = 10.0f,
						.children = {
							new WColumn({
								.spacing = 10.0f,
								.children = {
									new WConstraint({
										.axis = EAxis::Horizontal,
										.size = 300.0f,
										.child = new WSceneExplorer(),
									}),
									new WConstraint({
										.axis = EAxis::Vertical,
										.size = 200.0f,
										.child = new WAssetExplorer(),
									}),
									new WBox({
										.size = { 340.0f, 200.0f },
										.child = new WAssetExplorer(),
									}),
								},
							}),
							new WWrap({
								.axis = EAxis::Horizontal,
								.child = new WStack({
									.children = {
										new WColor({ Colors::Magenta.WithAlpha(0.2f) }),
										new WPadding({
											.padding = WPadding::Padding::All(8.0f),
											.child = new WColumn({
												.spacing = 4.0f,
												.children = {
													new WTestBox(),
													new WVisualizerWindow(),
													new WPropertiesWindow(),
												},
											}),
										}),
									},
								}),
							}),
						},
					}),
				}),
			},
		}),
	});
}
