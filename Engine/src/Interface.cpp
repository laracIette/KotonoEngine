#include "Interface.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ImageTextureManager.h>
#include <kotono_common/Path.h>
#include <kotono_framework/Shader.h>
#include "Engine.h"
#include "InterfaceImageObject.h"
#include "InterfaceImageComponent.h"
#include "ObjectManager.h"

void KInterface::Init()
{
	UPtr image1{ Engine.ObjectManager().Create<RInterfaceImageObject>() };
	UPtr image2{ Engine.ObjectManager().Create<RInterfaceImageObject>() };
	image1->GetImageComponent()->SetScreenSize({ 1024.0f, 1024.0f });
	image2->GetImageComponent()->SetScreenSize({ 1024.0f, 1024.0f });

	auto* shader2D{ Framework.ShaderManager().Get(Framework.Path().Framework() / R"(shaders\shader2D.ktshader)") };
	shader2D->SetName("2D Shader");

	auto* imageTexture1{ Framework.ImageTextureManager().Get(Framework.Path().Framework() / R"(assets\models\viking_room.png)") };
	auto* imageTexture2{ Framework.ImageTextureManager().Get(Framework.Path().Framework() / R"(assets\textures\default_texture.jpg)") };

	image1->RootComponent()->SetScreenSize({ 1024.0f, 1024.0f });
	image1->RootComponent()->SetRelativeScale({ 0.25f, 0.25f });
	image1->GetImageComponent()->SetShader(shader2D);
	image1->GetImageComponent()->SetImageTexture(imageTexture1);
	//image1->RootComponent()->SetAnchor(EAnchor::TopLeft);

	image2->RootComponent()->SetScreenSize({ 1024.0f, 1024.0f });
	image2->RootComponent()->SetRelativeScale({ 0.1f, 0.1f });
	image2->GetImageComponent()->SetShader(shader2D);
	image2->GetImageComponent()->SetImageTexture(imageTexture2);
#if true
	image2->SetParent(image1, ECoordinateSpace::World);
#else
	image2->SetLayer(1);
#endif
}