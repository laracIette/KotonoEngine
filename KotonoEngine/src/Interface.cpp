#include "Interface.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ImageTextureManager.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Shader.h>
#include <kotono_framework/WindowViewport.h>
#include "Engine.h"
#include "InterfaceImageObject.h"
#include "InterfaceImageComponent.h"
#include "InterfaceLayoutObject.h"
#include "ObjectManager.h"
#include "Timer.h"
#include "log.h"

void SInterface::Init()
{
	//layout_ = Engine.ObjectManager().Create<RInterfaceLayoutObject>();

	auto* image1 = Engine.ObjectManager().Create<RInterfaceImageObject>();
	auto* image2 = Engine.ObjectManager().Create<RInterfaceImageObject>();
	image1->GetImageComponent()->SetScreenSize(glm::vec2(1024.0f, 1024.0f));
	image2->GetImageComponent()->SetScreenSize(glm::vec2(1024.0f, 1024.0f));

	auto* shader2D = Framework.ShaderManager().Get(Framework.Path().GetFrameworkPath() / R"(shaders\shader2D.ktshader)");
	shader2D->SetName("2D Shader");

	auto* imageTexture1 = Framework.ImageTextureManager().Get(Framework.Path().GetSolutionPath() / R"(assets\models\viking_room.png)");
	auto* imageTexture2 = Framework.ImageTextureManager().Get(Framework.Path().GetSolutionPath() / R"(assets\textures\default_texture.jpg)");

#if true
	image1->GetRootComponent()->SetScreenSize(glm::vec2(1024.0f, 1024.0f));
	image1->GetRootComponent()->SetRelativeScale(glm::vec2(0.25f));
	image1->GetImageComponent()->SetShader(shader2D);
	image1->GetImageComponent()->SetImageTexture(imageTexture1);
	//image1->GetRootComponent()->SetAnchor(EAnchor::TopLeft);
	Engine.ObjectManager().selectedObject = image1;

	image2->GetRootComponent()->SetScreenSize(glm::vec2(1024.0f, 1024.0f));
	image2->GetRootComponent()->SetRelativeScale(glm::vec2(0.10f));
	image2->GetImageComponent()->SetShader(shader2D);
	image2->GetImageComponent()->SetImageTexture(imageTexture2);
#if true
	image2->SetParent(image1, ECoordinateSpace::World);
#else
	image2->SetLayer(1);

	auto* interfaceObjectStack = Engine.ObjectManager().Create<KInterfaceRowComponent>();
	interfaceObjectStack->SetOrientation(EOrientation::Horizontal);
	interfaceObjectStack->SetItemSpacing(0.1f);
	interfaceObjectStack->AddItem(image1);
	interfaceObjectStack->AddItem(image2);
#endif
#endif
}

void SInterface::Cleanup()
{
}