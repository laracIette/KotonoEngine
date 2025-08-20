#include "Test.h"
#include "widgets.h"
#include "log.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Viewport.h>

WTest::WTest()
{
	WWidget* widget{ 
		new WCenter({
			.direction = WCenter::Direction::All,
			.child = new WStack({
				.children = {
					new WColor({
						.color = { 0.5f, 0.5f, 0.5f, 0.5f },
					}),

					new WPadding({
						.padding = { 100.0f, 100.0f },
						.child = new WStack({
							.children = {
								new WColor({
									.color = { 0.3f, 0.3f, 0.3f, 0.3f },
								}),

								new WBox({
									.size = { 200.0f, 500.0f },
									.child = new WStack({
										.children = {
											new WColor({
												.color = { 1.0f, 0.0f, 0.0f, 0.5f },
											}),

											new WColumn({
												.spacing = 1.0f,
												.children = {
													new WBox({
														.size = { 100.0f, 50.0f },
														.child = new WButton({
															.onPress = []() { KT_LOG_KI(KT_LOG_COMPILE_TIME_LEVEL, "Test"); },
															.child = new WImage({
																.path = Framework.GetPath().GetSolutionPath() / R"(assets\textures\white_texture.jpg)",
															}),
														}),
													}),

													new WBox({
														.size = { 50.0f, 50.0f },
														.child = new WImage({
															.path = Framework.GetPath().GetSolutionPath() / R"(assets\textures\white_texture.jpg)",
														}),
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
		})
	};
	
	widget->Build({
		.position = { 0.0f, 0.0f },
		.bounds = glm::vec2(WindowViewport.GetExtent()),
		.layer = 0,
	});
}
