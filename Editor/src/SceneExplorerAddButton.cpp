#include "SceneExplorerAddButton.h"

#include <kotono_core/MeshComponent.h>
#include <kotono_interface/widgets.h>
#include <kotono_object/Scene.h>
#include <kotono_object/SceneComponent.h>
#include <kotono_object/SceneObject.h>

WidgetPtr WSceneExplorerAddButton::Build()
{
	UPtr color{ UCreate<WColor>{}() };
	color->SetColor(Colors::Green.WithValue(0.8f));

	UPtr text{ UCreate<WText>{}() };
	text->SetFontSize({ 22.0f, 26.0f });
	text->SetText("Add");

	UPtr button{ UCreate<WButton>{}() };
	button->SetOnPressed([this]() {
		UPath const shader{ "${ENGINE_DIRECTORY}/Graphics/assets/shaders/gbuffer.kasset" };
		UPath const model1{ "${ENGINE_DIRECTORY}/Graphics/assets/models/viking_room.obj" };
		UPath const model2{ "${ENGINE_DIRECTORY}/Graphics/assets/models/column.obj" };

		UPtr mesh{ UCreate<TSceneObject>{}() };
		UPtr rootComponent{ UCreate<KSceneComponent>{}() };
		UPtr meshComponent1{ UCreate<KMeshComponent>{}() };
		UPtr meshComponent2{ UCreate<KMeshComponent>{}() };
													
		mesh->AddComponent(rootComponent);
		rootComponent->SetRelativePosition(glm::vec3(0.0f));
													
		mesh->AddComponent(meshComponent1);
		meshComponent1->SetShader(shader);
		meshComponent1->SetModel(model1);
		meshComponent1->SetParent(rootComponent, ECoordinateSpace::Relative);
													
		mesh->AddComponent(meshComponent2);
		meshComponent2->SetShader(shader);
		meshComponent2->SetModel(model2);
		meshComponent2->SetParent(meshComponent1, ECoordinateSpace::Relative);
		meshComponent2->SetRelativePosition({ 1.0f, 1.0f, 1.0f });

		GetScene()->Add(mesh);
		mesh->Spawn();
	});

	const auto widgetTree{ UChildrenOwnerTree{ UCreate<WStack>{ "Add Button Stack" }(), {
		new UWidgetTreeLeaf{ button },
		new UChildOwnerTree{ UCreate<WCenter>{}(),
			new UWidgetTreeLeaf{ text },
		},
		new UWidgetTreeLeaf{ color },
	} } };
	widgetTree.Link();

	return widgetTree.Widget();
}

#include "generated/SceneExplorerAddButton.generated.inl"
