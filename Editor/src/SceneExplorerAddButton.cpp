#include "SceneExplorerAddButton.h"
#include <kotono_common/AssetManager.h>
#include <kotono_core/Game.h>
#include <kotono_core/Scene.h>
#include <kotono_core/SceneComponent.h>
#include <kotono_core/SceneMeshComponent.h>
#include <kotono_core/SceneObject.h>
#include <kotono_graphics/Model.h>
#include <kotono_graphics/Shader.h>
#include <kotono_interface/widgets.h>
#include <kotono_object/ObjectManager.h>

WidgetPtr WSceneExplorerAddButton::Build()
{
	UPtr color{ UCreate<WColor>{}() };
	color->SetColor(Colors::Green);

	UPtr button{ UCreate<WButton>{}() };
	button->SetOnPressed([]() {
		if (UPtr scene{ Game.GetOpenedScene() })
		{
			UAsset shader{ UAssetManager<UShader>::Get("${ENGINE_DIRECTORY}/Graphics/shaders/pbr.ktshader") };
			UAsset model1{ UAssetManager<UModel>::Get("${ENGINE_DIRECTORY}/Graphics/assets/models/viking_room.obj") };
			UAsset model2{ UAssetManager<UModel>::Get("${ENGINE_DIRECTORY}/Graphics/assets/models/column.obj") };

			UPtr mesh{ UCreate<TSceneObject>{}() };
			UPtr rootComponent{ UCreate<KSceneComponent>{}() };
			UPtr meshComponent1{ UCreate<KSceneMeshComponent>{}() };
			UPtr meshComponent2{ UCreate<KSceneMeshComponent>{}() };
													
			rootComponent->SetOwner(mesh);
			rootComponent->SetRelativePosition(glm::vec3(0.0f));
													
			meshComponent1->SetOwner(mesh);	
			meshComponent1->SetShader(shader);
			meshComponent1->SetModel(model1);
			meshComponent1->SetParent(rootComponent, ECoordinateSpace::Relative);
													
			meshComponent2->SetOwner(mesh);	
			meshComponent2->SetShader(shader);
			meshComponent2->SetModel(model2);
			meshComponent2->SetParent(meshComponent1, ECoordinateSpace::Relative);
			meshComponent2->SetRelativePosition({ 1.0f, 1.0f, 1.0f });

			scene->Add(mesh);
			mesh->Spawn();
		}
	});

	UPtr stack{ UCreate<WStack>{}() };
	stack->SetChildren({ color, button });

	return stack;
}

#include "generated/SceneExplorerAddButton.generated.inl"
