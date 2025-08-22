#include "Test.h"
#include "widgets.h"
#include "MyColumn.h"

WWidget* WTest::Build()
{
	return new WCenter({
		.axis = WCenter::Axis::All,
		.child = new WStack({
			.children = {
				new WColor({
					.color = KtColor::White().WithValue(0.01f),
				}),

				new WPadding({
					.padding = WPadding::Padding::All(16.0f),
					.child = new WStack({
						.children = {
							new WColor({
								.color = KtColor::Red(),
							}),

							new WPadding({
								.padding = WPadding::Padding::All(16.0f),
								.child = new WStack({
									.children = {
										new WColor({
											.color = KtColor::Green(),
										}),

										new WPadding({
											.padding = WPadding::Padding::All(16.0f),
											.child = new WStack({
												.children = {
													new WColor({
														.color = KtColor::Blue(),
													}),

													new WRow({
														.spacing = 5.0f,
														.children = {
															new WMyColumn({ .color = KtColor::Yellow() }),

															new WMyColumn({ .color = KtColor::Black() }),
														},
													}),
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
