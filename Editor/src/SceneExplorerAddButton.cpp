#include "SceneExplorerAddButton.h"
#include <kotono_common/AssetManager.h>
#include <kotono_core/Game.h>
#include <kotono_core/Scene.h>
#include <kotono_core/SceneComponent.h>
#include <kotono_core/SceneMeshComponent.h>
#include <kotono_core/SceneObject.h>
#include <kotono_graphics/Model.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/ShaderManager.h>
#include <kotono_interface/widgets.h>
#include <kotono_object/ObjectManager.h>

WidgetPtr WSceneExplorerAddButton::Build()
{
    return new WStack({
		.children = {
			new WColor({ Colors::Green }),
			new WButton({
				.onPressed = []() {
					if (UPtr scene{ Game.GetOpenedScene() })
					{
						auto* shader3D{ ShaderManager.Get("${ENGINE_DIRECTORY}/Graphics/shaders/shader3D.ktshader")};
						auto* model1{ UAssetManager<KtModel>::Get("${ENGINE_DIRECTORY}/Graphics/assets/models/viking_room.obj") };
						auto* model2{ UAssetManager<KtModel>::Get("${ENGINE_DIRECTORY}/Graphics/assets/models/column.obj") };
													
						UPtr mesh{ Create<TSceneObject>() };
						UPtr rootComponent{ Create<KSceneComponent>() };
						UPtr meshComponent1{ Create<KSceneMeshComponent>() };
						UPtr meshComponent2{ Create<KSceneMeshComponent>() };
													
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
	});
}
