#include "Test.h"
#include "Column.h"
#include "Image.h"
#include "Center.h"
#include "Padding.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Viewport.h>

WTest::WTest()
{
	WWidget* widget{ 
		new WCenter(
			{
			},
			{
				.direction = WCenter::Direction::Horizontal,
				.child = new WPadding(
					{
					},
					{
						.padding = { 100.0f, 100.0f },
						.child = new WColumn(
							{
							},
							{
								.spacing = 450.0f,
								.children = {
									new WImage(
										{
										},
										{
											.size = { 100.0f, 50.0f },
											.path = Framework.GetPath().GetSolutionPath() / R"(assets\textures\white_texture.jpg)",
										}
									),
									new WImage(
										{
										},
										{
											.size = { 50.0f, 50.0f },
											.path = Framework.GetPath().GetSolutionPath() / R"(assets\textures\white_texture.jpg)",
										}
									),
								}
							}
						),
					}
				)
			}
		)
	};
	
	widget->Build(
		{
			.position = { 0.0f, 0.0f },
			.bounds = glm::vec2(WindowViewport.GetExtent()),
			.layer = 0,
		}
	);
}
