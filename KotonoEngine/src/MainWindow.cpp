#include "MainWindow.h"
#include <kotono_interface/widgets.h>
#include "Engine.h"
#include "Timer.h"
#include "UpdateTimeText.h"
#include "DrawTimeText.h"

WWidget* WMainWindow::Build()
{
	updateTimer_ = Engine.GetObjectManager().Create<KTimer>();
	updateTimer_->SetDuration(UDuration::FromSeconds(1.0f / 8.0f));
	updateTimer_->SetIsRepeat(true);
	updateTimer_->Start();

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
								.child = new WStack({
									.children = {
										new WColor({ KtColor::Green() }),
										new WPadding({
											.padding = WPadding::Padding::All(16.0f),
											.child = new WStack({
												.children = {
													new WColor({ KtColor::Red() }),
													new WColumn({
														.children = {
															new WUpdateTimeText(),
															new WDrawTimeText(),
														},
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

void WMainWindow::Cleanup()
{
	updateTimer_->Delete();

	WWidget::Cleanup();
}
