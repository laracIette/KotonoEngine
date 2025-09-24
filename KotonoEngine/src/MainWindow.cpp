#include "MainWindow.h"
#include <kotono_interface/widgets.h>
#include "Engine.h"
#include "Timer.h"
#include "UpdateTimeText.h"
#include "DrawTimeText.h"
#include "TestBox.h"

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
				new WExpanded({
					.child = new WRow({
						.spacing = 10.0f,
						.children = {
							new WBox({
								.size = { 200.0f, 500.0f },
								.child = new WColor({ KtColor::Red() }),
							}),
							new WExpanded({
								.child = new WStack({
									.children = {
										new WColor({ KtColor::Green().WithAlpha(0.5f) }),
										new WPadding({
											.padding = WPadding::Padding::All(16.0f),
											.child = new WStack({
												.children = {
													new WColor({ KtColor::Magenta().WithAlpha(0.5f) }),
													new WPadding({
														.padding = WPadding::Padding::All(8.0f),
														.child = new WTestBox(),
													}),
												},
											}), 
										}),
									},
								}),
							}),
							new WBox({
								.size = { 200.0f, 500.0f },
								.child = new WColor({ KtColor::Red() }),
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
