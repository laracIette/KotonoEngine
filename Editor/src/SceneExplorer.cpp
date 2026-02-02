#include "SceneExplorer.h"
#include "SceneExplorerItem.h"
#include "GameManager.h"
#include <kotono_core/Game.h>
#include <kotono_core/ObjectManager.h>
#include <kotono_core/Scene.h>
#include <kotono_core/SceneComponent.h>
#include <kotono_core/SceneMeshComponent.h>
#include <kotono_core/SceneObject.h>
#include <kotono_graphics/Model.h>
#include <kotono_graphics/ModelManager.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/ShaderManager.h>
#include <kotono_interface/widgets.h>

WWidget* WSceneExplorer::Build()
{
	GameManager.EventStateChanged().AddListener(this, &WSceneExplorer::OnGameStateChanged);

	if (UPtr scene{ Game.GetOpenedScene() })
	{
		scene->EventSceneObjectsUpdated().AddListener(this, &WSceneExplorer::Refresh);
	}

	return new WStack({
		.children = {
			new WColor({ UColor::White().WithValue(0.5f).WithAlpha(0.8f) }),
			new WPadding({
				.padding = WPadding::Padding::All(10.0f),
				.child = new WColumn({
					.spacing = 10.0f,
					.children = {
						// Create scene object button
						new WBox({ 
							.size = { 80.0f, 40.0f },
							.child = new WStack({
								.children = {
									new WColor({ UColor::Green() }),
									new WButton({
										.onPress = []() {
											if (UPtr scene{ Game.GetOpenedScene() })
											{
												auto* shader3D{ ShaderManager.Get("${ENGINE_DIRECTORY}/Graphics/shaders/shader3D.ktshader")};
												auto* model1{ ModelManager.Get("${ENGINE_DIRECTORY}/Graphics/assets/models/viking_room.obj") };
												auto* model2{ ModelManager.Get("${ENGINE_DIRECTORY}/Graphics/assets/models/column.obj") };
													
												UPtr mesh{ ObjectManager.Create<TSceneObject>() };
												UPtr rootComponent{ ObjectManager.Create<KSceneComponent>() };
												UPtr meshComponent1{ ObjectManager.Create<KSceneMeshComponent>() };
												UPtr meshComponent2{ ObjectManager.Create<KSceneMeshComponent>() };
													
												rootComponent->SetOwner(mesh);
												rootComponent->SetRelativePosition(glm::vec3(0.0f));
													
												meshComponent1->SetOwner(mesh);	
												meshComponent1->SetShader(shader3D);
												meshComponent1->SetModel(model1);
												meshComponent1->SetParent(rootComponent, ECoordinateSpace::Relative);
													
												meshComponent2->SetOwner(mesh);	
												meshComponent2->SetShader(shader3D);
												meshComponent2->SetModel(model2);
												meshComponent2->SetParent(meshComponent1, ECoordinateSpace::Relative);
												meshComponent2->SetRelativePosition({ 1.0f, 1.0f, 1.0f });

												scene->Add(mesh);
												mesh->Spawn();
											}
										},
									}),
								},
							}),
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
									new WColor({ UColor::Black().WithAlpha(0.2f) }),
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

	WWidget::Cleanup();
}

void WSceneExplorer::OnGameStateChanged(const EGameState gameState)
{
	Refresh();
}
