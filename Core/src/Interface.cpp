#include "Interface.h"
#include <kotono_graphics/ShaderManager.h>
#include <kotono_graphics/ImageTextureManager.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/Shader.h>
#include "InterfaceImageObject.h"
#include "InterfaceImageComponent.h"
#include "ObjectManager.h"

void KInterface::Init()
{
	UPtr image1{ ObjectManager.Create<RInterfaceImageObject>() };
	UPtr image2{ ObjectManager.Create<RInterfaceImageObject>() };
	image1->ImageComponent()->SetScreenSize({ 1024.0f, 1024.0f });
	image2->ImageComponent()->SetScreenSize({ 1024.0f, 1024.0f });

	auto* shader2D{ ShaderManager.Get(KtPath::Graphics() / R"(shaders\shader2D.ktshader)") };
	shader2D->SetName("2D Shader");

	auto* imageTexture1{ ImageTextureManager.Get(KtPath::Graphics() / R"(assets\models\viking_room.png)") };
	auto* imageTexture2{ ImageTextureManager.Get(KtPath::Graphics() / R"(assets\textures\default_texture.jpg)") };

	image1->RootComponent()->SetScreenSize({ 1024.0f, 1024.0f });
	image1->RootComponent()->SetRelativeScale({ 0.25f, 0.25f });
	image1->ImageComponent()->SetShader(shader2D);
	image1->ImageComponent()->SetImageTexture(imageTexture1);
	//image1->RootComponent()->SetAnchor(EAnchor::TopLeft);

	image2->RootComponent()->SetScreenSize({ 1024.0f, 1024.0f });
	image2->RootComponent()->SetRelativeScale({ 0.1f, 0.1f });
	image2->ImageComponent()->SetShader(shader2D);
	image2->ImageComponent()->SetImageTexture(imageTexture2);
#if true
	image2->SetParent(image1, ECoordinateSpace::World);
#else
	image2->SetLayer(1);
#endif
}