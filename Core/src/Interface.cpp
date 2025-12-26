#include "Interface.h"
#include "InterfaceComponent.h"
#include "InterfaceImageComponent.h"
#include "InterfaceObject.h"
#include "ObjectManager.h"
#include <kotono_common/Path.h>
#include <kotono_graphics/ImageTextureManager.h>
#include <kotono_graphics/Shader.h>
#include <kotono_graphics/ShaderManager.h>

KInterface::KInterface(UPtrOwnerBase* ptrOwner) : 
	Base(ptrOwner)
{
	auto* shader2D{ ShaderManager.Get(KtPath::Graphics() / "shaders" / "shader2D.ktshader") };
	auto* imageTexture1{ ImageTextureManager.Get(KtPath::Graphics() / "assets" / "models" / "viking_room.png") };
	auto* imageTexture2{ ImageTextureManager.Get(KtPath::Graphics() / "assets" / "textures" / "default_texture.jpg") };

	UPtr image1{ ObjectManager.Create<RInterfaceObject>() };
	UPtr image2{ ObjectManager.Create<RInterfaceObject>() };
	UPtr rootComponent1{ ObjectManager.Create<KInterfaceComponent>() };
	UPtr rootComponent2{ ObjectManager.Create<KInterfaceComponent>() };
	UPtr imageComponent1{ ObjectManager.Create<KInterfaceImageComponent>() };
	UPtr imageComponent2{ ObjectManager.Create<KInterfaceImageComponent>() };

	rootComponent1->SetOwner(image1);
	rootComponent2->SetOwner(image2);

	imageComponent1->SetOwner(image1);
	imageComponent1->SetShader(shader2D);
	imageComponent1->SetImageTexture(imageTexture1);
	imageComponent1->SetParent(rootComponent1, ECoordinateSpace::Relative);
	imageComponent1->SetScreenSize({ 1024.0f, 1024.0f });
	imageComponent1->SetRelativeScale({ 0.25f, 0.25f });

	imageComponent2->SetOwner(image2);
	imageComponent2->SetShader(shader2D);
	imageComponent2->SetImageTexture(imageTexture2);
	imageComponent2->SetParent(rootComponent2, ECoordinateSpace::Relative);
	imageComponent2->SetScreenSize({ 1024.0f, 1024.0f });
	imageComponent2->SetRelativeScale({ 0.1f, 0.1f });

	image2->SetParent(image1, ECoordinateSpace::World);

	interfaceObjects_.Append({ image1, image2 });
}

void KInterface::Cleanup()
{
	for (const auto& interfaceObject : interfaceObjects_)
	{
		if (interfaceObject)
		{
			interfaceObject->Delete();
		}
	}
	interfaceObjects_.Clear();

	Base::Cleanup();
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
