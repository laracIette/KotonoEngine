#include "SceneExplorer.h"
#include "GameManager.h"
#include "SceneExplorerAddButton.h"
#include "SceneExplorerItem.h"
#include "SceneExplorerRemoveButton.h"
#include <kotono_core/Game.h>
#include <kotono_core/Scene.h>
#include <kotono_interface/widgets.h>

WidgetPtr WSceneExplorer::Build()
{
	GameManager.EventStateChanged().AddListener(this, &WSceneExplorer::OnGameStateChanged);

	if (UPtr scene{ Game.GetOpenedScene() })
	{
		scene->EventSceneObjectsUpdated().AddListener(this, &WSceneExplorer::Refresh);
	}

	return new WStack({
		.children = {
			new WColor({ Colors::White.WithValue(0.5f).WithAlpha(0.8f) }),
			new WPadding({
				.padding = WPadding::Padding::All(10.0f),
				.child = new WColumn({
					.spacing = 10.0f,
					.children = {
						new WRow({
							.children = {
								new WBox({
									.size = { 80.0f, 40.0f },
									.child = new WSceneExplorerAddButton(),
								}),
								new WSpacer({}),
								new WBox({
									.size = { 80.0f, 40.0f },
									.child = new WSceneExplorerRemoveButton(),
								}),
							},
						}),
						
						new WText({
							.text = "Scene Explorer",
							.fontSize = { 20.0f, 24.0f },
							.spacing = -5.0f,
						}),
						new WPadding({
							.padding = WPadding::Padding::All(5.0f),
							.child = new WStack({
								.children = {
									new WColor({ Colors::Black.WithAlpha(0.2f) }),
									new WPadding({
										.padding = WPadding::Padding::All(5.0f),
										.child = new WList({ 
											.children = []() {
												WidgetVector result{};
												if (UPtr scene{ Game.GetOpenedScene() })
												{
													for (const auto& sceneObject : scene->SceneObjects())
													{
														result.push_back(new WSceneExplorerItem(sceneObject));
													}
												}
												return result;
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
	});
}

void WSceneExplorer::Cleanup()
{
	if (UPtr scene{ Game.GetOpenedScene() })
	{
		scene->EventSceneObjectsUpdated().RemoveListener(this, &WSceneExplorer::Refresh);
	}

	GameManager.EventStateChanged().RemoveListener(this, &WSceneExplorer::OnGameStateChanged);

	Base::Cleanup();
}

void WSceneExplorer::OnGameStateChanged(const EGameState gameState)
{
	Refresh();
}

#include "generated/SceneExplorer.generated.inl"
