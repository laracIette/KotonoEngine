#include "Test.h"
#include "widgets.h"
#include "log.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Viewport.h>

WTest::WTest()
{
	WWidget* widget{ 
		new WCenter({
			.axis = WCenter::Axis::All,
			.child = new WStack({
				.children = {
					new WColor({
						.color = { 0.01f, 0.01f, 0.01f, 1.0f },
					}),

					new WPadding({
						.padding = WPadding::All(16.0f),
						.child = new WStack({
							.children = {
								new WColor({
									.color = { 1.0f, 0.0f, 0.0f, 1.0f },
								}),

								new WPadding({
									.padding = WPadding::All(16.0f),
									.child = new WStack({
										.children = {
											new WColor({
												.color = { 0.0f, 1.0f, 0.0f, 1.0f },
											}),

											new WPadding({
												.padding = WPadding::All(16.0f),
												.child = new WStack({
													.children = {
														new WColor({
															.color = { 0.0f, 0.0f, 1.0f, 1.0f },
														}),

														new WRow({
															.spacing = 5.0f,
															.children = {
																new WColumn({
																	.spacing = 5.0f,
																	.children = {
																		new WPadding({
																			.padding = WPadding::All(16.0f),
																			.child = new WColor({
																				.color = { 1.0f, 1.0f, 0.0f, 1.0f },
																			}),
																		}),
																	},
																}),

																new WColumn({
																	.spacing = 5.0f,
																	.children = {
																		new WPadding({
																			.padding = WPadding::All(16.0f),
																			.child = new WColor({
																				.color = { 1.0f, 1.0f, 0.0f, 1.0f },
																			}),
																		}),
																	},
																}),
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
		})
	};
	
	widget->Build({
		.position = { 0.0f, 0.0f },
		.bounds = glm::vec2(WindowViewport.GetExtent()),
		.layer = 0,
	});
}
