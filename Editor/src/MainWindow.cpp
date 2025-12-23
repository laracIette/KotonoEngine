#include "MainWindow.h"
#include <kotono_interface/widgets.h>
#include <kotono_core/Timer.h>
#include "UpdateTimeText.h"
#include "DrawTimeText.h"
#include "TestBox.h"
#include "VisualizerWindow.h"
#include "PropertiesWindow.h"
#include "GameStateButton.h"
#include "SceneExplorer.h"

WWidget* WMainWindow::Build()
{
	return new WPadding({
		.padding = WPadding::Padding::All(16.0f),
		.child = new WColumn({
			.spacing = 5.0f,
			.children = {
				new WRow({
					.spacing = 4.0f,
					.children = {
						new WRow({
							.spacing = 5.0f,
							.children = {
								new WBox({
									.size = { 50.0f, 50.0f },
									.child = new WColor({ KtColor::Green() }),
								}),
								new WBox({
									.size = { 50.0f, 50.0f },
									.child = new WColor({ KtColor::Green() }),
								}),
							},
						}),
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
							new WBox({ 
								.size = { 300.0f, 500.0f },
								.child = new WSceneExplorer(),
							}),
							
							new WBox({
								.size = { 300.0f, 300.0f },
								.child = new WStack({
									.children = {
										new WColor({ KtColor::Red() }),
										new WList({
											.spacing = 75.0f,
											.children = []() {
												return WidgetVector{
													new WText({
														.text = "Tesstt",
														.spacing = -20.0f,
													}),
													new WText({
														.text = "Testett",
														.spacing = -20.0f,
													}),
													new WText({
														.text = "Tstestt",
														.spacing = -20.0f,
													}),
													new WText({
														.text = "Tstetestsestt",
														.spacing = -20.0f,
													}),
													new WText({
														.text = "Tststsestt",
														.spacing = -20.0f,
													}),
													new WText({
														.text = "Tstststt",
														.spacing = -20.0f,
													}),
												};
											}
										}),
									},
								}),
							}),
							new WExpanded({}),
							new WBox({
								.size = { 400.0f, 800.0f },
								.child = new WStack({
									.children = {
										new WColor({ KtColor::Magenta().WithAlpha(0.2f) }),
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
				new WRow({
					.spacing = 15.0f,
					.children = {
						new WBox({
							.size = { 100.0f, 50.0f },
							.child = new WColor({ KtColor::Blue() }),
						}),
						new WBox({
							.size = { 100.0f, 50.0f },
							.child = new WColor({ KtColor::Blue() }),
						}),
						new WBox({
							.size = { 100.0f, 50.0f },
							.child = new WColor({ KtColor::Blue() }),
						}),
					},
				}),
			},
		}),
	});
}
