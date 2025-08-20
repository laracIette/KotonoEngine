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
						.padding = { 16.0f, 16.0f, 16.0f, 16.0f },
						.child = new WStack({
							.children = {
								new WColor({
									.color = { 0.5f, 0.5f, 0.5f, 1.0f },
								}),

								new WRow({
									.spacing = 5.0f,
									.children = {
										new WPadding({
											.padding = { 4.0f, 4.0f, 4.0f, 4.0f },
											.child = new WStack({
												.children = {
													new WColor({
														.color = { 1.0f, 0.0f, 0.0f, 1.0f },
													}),
													new WPadding({
														.padding = { 8.0f, 8.0f, 8.0f, 8.0f },
														.child = new WColumn({
															.spacing = 3.0f,
															.children = {
																new WColor({
																	.color = { 0.0f, 0.0f, 1.0f, 1.0f },
																}),
																new WColor({
																	.color = { 0.0f, 0.0f, 1.0f, 1.0f },
																}),
																new WColor({
																	.color = { 0.0f, 0.0f, 1.0f, 1.0f },
																}),
															},
														}),
													}),
												},
											}),
										}),
																
										new WPadding({
											.padding = { 4.0f, 4.0f, 4.0f, 4.0f },
											.child = new WStack({
												.children = {
													new WColor({
														.color = { 1.0f, 1.0f, 0.0f, 1.0f },
													}),
													new WPadding({
														.padding = { 8.0f, 8.0f, 8.0f, 8.0f },
														.child = new WColumn({
															.spacing = 30.0f,
															.children = {
																new WColor({
																	.color = { 0.0f, 1.0f, 1.0f, 1.0f },
																}),
																new WColor({
																	.color = { 0.0f, 1.0f, 1.0f, 1.0f },
																}),
																new WColor({
																	.color = { 0.0f, 1.0f, 1.0f, 1.0f },
																}),
															},
														}),
													}),
												},
											}),
										}),

										new WPadding({
											.padding = { 4.0f, 4.0f, 4.0f, 4.0f },
											.child = new WStack({
												.children = {
													new WColor({
														.color = { 1.0f, 1.0f, 1.0f, 1.0f },
													}),
													new WPadding({
														.padding = { 8.0f, 8.0f, 8.0f, 8.0f },
														.child = new WColumn({
															.spacing = 100.0f,
															.children = {
																new WColor({
																	.color = { 0.0f, 1.0f, 0.0f, 1.0f },
																}),
																new WColor({
																	.color = { 0.0f, 1.0f, 0.0f, 1.0f },
																}),
																new WColor({
																	.color = { 0.0f, 1.0f, 0.0f, 1.0f },
																}),
															},
														}),
													}),
												},
											}),
										}),
									},
								}),

								/*new WBounds({
									.size = { 200.0f, 500.0f },
									.child = new WStack({
										.children = {
											new WColor({
												.color = { 1.0f, 0.0f, 0.0f, 0.5f },
											}),

											new WColumn({
												.spacing = 1.0f,
												.children = {
													new WBounds({
														.size = { 100.0f, 50.0f },
														.child = new WButton({
															.onPress = []() { KT_LOG_KI(KT_LOG_COMPILE_TIME_LEVEL, "Test"); },
															.child = new WImage({
																.path = Framework.GetPath().GetSolutionPath() / R"(assets\textures\white_texture.jpg)",
															}),
														}),
													}),

													new WBounds({
														.size = { 50.0f, 50.0f },
														.child = new WImage({
															.path = Framework.GetPath().GetSolutionPath() / R"(assets\textures\white_texture.jpg)",
														}),
													}),
												},
											}),
										},
									}),
								}),*/
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
