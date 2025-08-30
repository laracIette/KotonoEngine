#include "MainWindow.h"
#include <kotono_interface/widgets.h>

WWidget* WMainWindow::Build()
{
	return new WPadding({
		.padding = WPadding::Padding::All(16.0f),
		.child = new WRow({
			.spacing = 5.0f,
			.children = {
				new WExpanded({
					.child = new WColor({ KtColor::White() }),
				}),
				new WExpanded({
					.child = new WColor({ KtColor::White() }),
				}),
				new WExpanded({
					.child = new WColor({ KtColor::White() }),
				}),
			},
		}),
	});
}
