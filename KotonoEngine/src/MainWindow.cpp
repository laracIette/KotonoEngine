#include "MainWindow.h"
#include <kotono_interface/widgets.h>

WWidget* WMainWindow::Build()
{
	return new WPadding({
		.padding = WPadding::Padding::All(16.0f),
		.child = new WColumn({
			.spacing = 5.0f,
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
				new WExpanded({
					.child = new WRow({
						.spacing = 10.0f,
						.children = {
							new WBox({ 
								.size = { 200.0f, 500.0f },
								.child = new WColor({ KtColor::Red() }),
							}),
							new WExpanded({ 
								.child = new WColor({ KtColor::Red() }),
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
