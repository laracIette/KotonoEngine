#include "Interface.h"
#include "InterfaceComponent.h"
#include "InterfaceImageComponent.h"
#include "InterfaceObject.h"
#include <kotono_common/AssetManager.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/Texture.h>

KInterface::KInterface()
{
	UAsset shader{ SAssetManager<UShader>::Get("${ENGINE_DIRECTORY}/Graphics/assets/shaders/shader2D.kasset") };
	UAsset texture1{ SAssetManager<UTexture>::Get("${ENGINE_DIRECTORY}/Graphics/assets/models/viking_room.png") };
	UAsset texture2{ SAssetManager<UTexture>::Get("${ENGINE_DIRECTORY}/Graphics/assets/textures/default_texture.jpg") };

	UPtr image1{ UCreate<RInterfaceObject>{}() };
	UPtr image2{ UCreate<RInterfaceObject>{}() };
	UPtr rootComponent1{ UCreate<KInterfaceComponent>{}() };
	UPtr rootComponent2{ UCreate<KInterfaceComponent>{}() };
	UPtr imageComponent1{ UCreate<KInterfaceImageComponent>{}() };
	UPtr imageComponent2{ UCreate<KInterfaceImageComponent>{}() };

	rootComponent1->SetOwner(image1);
	rootComponent1->SetRelativePosition({ 0.0f, 0.5f });

	rootComponent2->SetOwner(image2);
	rootComponent2->SetRelativePosition({ 0.0f, 0.6f });

	imageComponent1->SetOwner(image1);
	imageComponent1->SetShader(shader);
	imageComponent1->SetTexture(texture1);
	imageComponent1->SetParent(rootComponent1, ECoordinateSpace::Relative);
	//imageComponent1->SetScreenSize({ 1024.0f, 1024.0f });
	imageComponent1->SetRelativeScale({ 0.25f, 0.25f });

	imageComponent2->SetOwner(image2);
	imageComponent2->SetShader(shader);
	imageComponent2->SetTexture(texture2);
	imageComponent2->SetParent(rootComponent2, ECoordinateSpace::Relative);
	//imageComponent2->SetScreenSize({ 1024.0f, 1024.0f });
	imageComponent2->SetRelativeScale({ 0.1f, 0.1f });

	image2->SetParent(image1, ECoordinateSpace::World);

	interfaceObjects_.Append({ image1, image2 });
}

KInterface::~KInterface()
{
	for (const auto& interfaceObject : interfaceObjects_)
	{
		if (interfaceObject)
		{
			interfaceObject->Delete();
		}
	}
}

void KInterface::SpawnInterfaceObjects()
{
	for (auto& interfaceObject : interfaceObjects_)
	{
		interfaceObject->Spawn();
	}
}

void KInterface::Update(const float deltaTime)
{
	for (auto& interfaceObject : interfaceObjects_)
	{
		if (interfaceObject->GetCanUpdate())
		{
			interfaceObject->Update(deltaTime);
		}

		if (!interfaceObject->isInit_)
		{
			interfaceObject->Init();
			interfaceObject->isInit_ = true;
		}

		interfaceObject->InitInterfaceComponents();
		interfaceObject->UpdateInterfaceComponents(deltaTime);
	}
}

#include "generated/Interface.generated.inl"
