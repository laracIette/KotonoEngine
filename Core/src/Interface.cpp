#include "Interface.h"
#include "InterfaceComponent.h"
#include "InterfaceImageComponent.h"
#include "InterfaceObject.h"
#include <kotono_common/AssetManager.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/ShaderManager.h>
#include <kotono_graphics/Texture.h>
#include <kotono_object/ObjectManager.h>

KInterface::KInterface(UPtrOwnerBase* ptrOwner) 
	: Base(ptrOwner)
{
	auto* shader2D{ ShaderManager.Get("${ENGINE_DIRECTORY}/Graphics/shaders/shader2D.ktshader") };
	auto* texture1{ UAssetManager<KtTexture>::Get("${ENGINE_DIRECTORY}/Graphics/assets/models/viking_room.png") };
	auto* texture2{ UAssetManager<KtTexture>::Get("${ENGINE_DIRECTORY}/Graphics/assets/textures/default_texture.jpg") };

	UPtr image1{ Create<RInterfaceObject>() };
	UPtr image2{ Create<RInterfaceObject>() };
	UPtr rootComponent1{ Create<KInterfaceComponent>() };
	UPtr rootComponent2{ Create<KInterfaceComponent>() };
	UPtr imageComponent1{ Create<KInterfaceImageComponent>() };
	UPtr imageComponent2{ Create<KInterfaceImageComponent>() };

	rootComponent1->SetOwner(image1);
	rootComponent1->SetRelativePosition({ 0.0f, 0.5f });

	rootComponent2->SetOwner(image2);
	rootComponent2->SetRelativePosition({ 0.0f, 0.6f });

	imageComponent1->SetOwner(image1);
	imageComponent1->SetShader(shader2D);
	imageComponent1->SetTexture(texture1);
	imageComponent1->SetParent(rootComponent1, ECoordinateSpace::Relative);
	//imageComponent1->SetScreenSize({ 1024.0f, 1024.0f });
	imageComponent1->SetRelativeScale({ 0.25f, 0.25f });

	imageComponent2->SetOwner(image2);
	imageComponent2->SetShader(shader2D);
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
