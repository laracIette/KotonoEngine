#include "SceneExplorer.h"
#include <kotono_common/Path.h>
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
	ObjectManager.EventSelectedObjectChanged().AddListener(KtDelegate(this, &WSceneExplorer::Refresh));

	offset_ += 1.0f;

	return new WStack({
		.children = {
			new WColor({ KtColor::White().WithValue(0.5f).WithAlpha(0.8f) }),
			new WPadding({
				.padding = WPadding::Padding::All(10.0f),
				.child = new WColumn({
					.spacing = 10.0f,
					.children = {
						new WBox({ 
							.size = { 80.0f, 40.0f },
							.child = new WStack({
								.children = {
									new WColor({ KtColor::Green() }),
									new WButton({
										.onPress = [this]() {
											if (UPtr scene{ Game.GetOpenedScene() })
											{
												SetState([this, scene]() {
													auto* shader3D{ ShaderManager.Get(KtPath::Graphics() / "shaders" / "shader3D.ktshader") };													
													auto* model{ ModelManager.Get(KtPath::Graphics() / "assets" / "models" / "viking_room.obj") };
													
													UPtr mesh{ ObjectManager.Create<TSceneObject>() };
													UPtr rootComponent{ ObjectManager.Create<KSceneComponent>() };
													UPtr meshComponent{ ObjectManager.Create<KSceneMeshComponent>() };
													
													rootComponent->SetOwner(mesh);
													meshComponent->SetOwner(mesh);
													
													rootComponent->SetRelativePosition({ offset_, offset_, offset_ });
													meshComponent->SetShader(shader3D);
													meshComponent->SetModel(model);
													meshComponent->SetParent(rootComponent, ECoordinateSpace::Relative);

													scene->Add(mesh);
													mesh->Spawn();
												});
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
									new WColor({ KtColor::Black().WithAlpha(0.2f) }),
									new WPadding({
										.padding = WPadding::Padding::All(5.0f),
										.child = new WList({ 
											.children = [this]() {
												WidgetVector result{};
												if (UPtr scene{ Game.GetOpenedScene() })
												{
													for (const auto& sceneObject : scene->SceneObjects())
													{
														result.push_back(new WConstraint({
															.axis = WConstraint::Axis::Vertical,
															.size = 30.0f,
															.child = new WStack({
																.children = {
																	ObjectManager.GetSelectedObject() == sceneObject
																		? new WColor({ KtColor::Black().WithAlpha(0.2f) })
																		: new WColor({ KtColor::Transparent() }),
																	new WButton({
																		.onPress = [this, sceneObject]() {
																			SetState([sceneObject]() {
																				ObjectManager.SetSelectedObject(sceneObject);
																			});
																		},
																	}),
																	new WText({
																		.text = sceneObject->GetName(),
																		.fontSize = { 20.0f, 24.0f },
																		.spacing = -3.0f,
																	}),
																},
															})
														}));
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
	ObjectManager.EventSelectedObjectChanged().RemoveListener(KtDelegate(this, &WSceneExplorer::Refresh));
	
	WWidget::Cleanup();
}
