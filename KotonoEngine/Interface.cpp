#include "Interface.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ImageTextureManager.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Viewport.h>
#include "Engine.h"
#include "InterfaceImageObject.h"
#include "InterfaceImageComponent.h"
#include "InterfaceTextBoxObject.h"
#include "InterfaceFloatTextBoxObject.h"
#include "InterfaceTextComponent.h"
#include "Timer.h"
#include "Time.h"
#include "math_utils.h"

void KInterface::Construct()
{
	Base::Construct();

	image1_ = AddObject<RInterfaceImageObject>();
	image2_ = AddObject<RInterfaceImageObject>();
	textBox1_ = AddObject<RInterfaceTextBoxObject>();
	textBox2_ = AddObject<RInterfaceFloatTextBoxObject>();
	textBox3_ = AddObject<RInterfaceFloatTextBoxObject>();
	image1_->GetImageComponent()->GetRect().SetScreenSize(&WindowViewport, glm::vec2(1024.0f, 1024.0f));
	image2_->GetImageComponent()->GetRect().SetScreenSize(&WindowViewport, glm::vec2(1024.0f, 1024.0f));
}

void KInterface::Init()
{
	Base::Init();

	SetLayout();
}

void KInterface::SetLayout()
{
	auto* shader2D = Framework.GetShaderManager().Get(Framework.GetPath().GetFrameworkPath() / R"(shaders\shader2D.ktshader)");
	shader2D->SetName("2D Shader");

	auto* imageTexture1 = Framework.GetImageTextureManager().Get(Framework.GetPath().GetSolutionPath() / R"(assets\models\viking_room.png)");
	auto* imageTexture2 = Framework.GetImageTextureManager().Get(Framework.GetPath().GetSolutionPath() / R"(assets\textures\default_texture.jpg)");

#if true
	image1_->GetRect().SetScreenSize(&WindowViewport, glm::vec2(1024.0f, 1024.0f));
	image1_->GetRect().SetRelativeScale(glm::vec2(0.25f));
	image1_->GetImageComponent()->SetShader(shader2D);
	image1_->GetImageComponent()->SetImageTexture(imageTexture1);
	//image1->GetRect().SetAnchor(EAnchor::TopLeft);

	image2_->GetRect().SetScreenSize(&WindowViewport, glm::vec2(1024.0f, 1024.0f));
	image2_->GetRect().SetRelativeScale(glm::vec2(0.10f));
	image2_->GetImageComponent()->SetShader(shader2D);
	image2_->GetImageComponent()->SetImageTexture(imageTexture2);
#if true
	image2_->SetParent(image1_, ECoordinateSpace::World);
#else
	image2_->SetLayer(1);

	auto* interfaceObjectStack = Engine.GetObjectManager().Create<KInterfaceStackComponent>();
	interfaceObjectStack->SetOrientation(EOrientation::Horizontal);
	interfaceObjectStack->SetItemSpacing(0.1f);
	interfaceObjectStack->AddItem(image1);
	interfaceObjectStack->AddItem(image2);
#endif
#endif
#if false
	auto text = Engine.GetObjectManager().Create<RInterfaceTextObject>();
	text->GetRect().SetRelativePosition(glm::vec2(0.2f, 0.0f));
	text->GetTextComponent()->SetFontSize(32.0f);
	text->GetTextComponent()->SetSpacing(0.05f);
	text->GetTextComponent()->SetShader(shader2D);
	text->GetTextComponent()->SetText("hello world !");
#else
	textBox1_->GetRect().SetRelativePosition(glm::vec2(0.5f, -0.85f));
	textBox1_->GetTextComponent()->SetFontSize(32.0f);
	textBox1_->GetTextComponent()->SetSpacing(0.05f);
	textBox1_->GetTextComponent()->SetShader(shader2D);
	//textBox1_->GetTextComponent()->SetText("plz updaaaate !"); // should be in construct cause creates objects
	textBox1_->GetTextComponent()->SetTextBinding([]() { return std::format("{} fps", round(1.0f / Engine.GetTime().GetDelta(), 2)); });


	textBox2_->GetRect().SetRelativePosition(glm::vec2(0.3f, 0.3f));
	textBox2_->GetTextComponent()->SetFontSize(32.0f);
	textBox2_->GetTextComponent()->SetSpacing(0.05f);
	textBox2_->GetTextComponent()->SetShader(shader2D);
	textBox2_->GetTextComponent()->SetTextBinding([this]() { return std::to_string(image1_->GetRect().GetScreenPosition(&WindowViewport).x); });
	ListenEvent(textBox2_->GetValueChangedEvent(), &KInterface::OnTextBox2ValueChanged);

	textBox3_->GetRect().SetRelativePosition(glm::vec2(0.3f, 0.6f));
	textBox3_->GetTextComponent()->SetFontSize(32.0f);
	textBox3_->GetTextComponent()->SetSpacing(0.05f);
	textBox3_->GetTextComponent()->SetShader(shader2D);
	textBox3_->GetTextComponent()->SetTextBinding([this]() { return std::to_string(image1_->GetRect().GetScreenPosition(&WindowViewport).y); });
	ListenEvent(textBox3_->GetValueChangedEvent(), &KInterface::OnTextBox3ValueChanged);
#endif
}

void KInterface::OnTextBox2ValueChanged(float delta)
{
	image1_->GetRect().AddOffset(glm::vec2(delta / 800.0f, 0.0f));
}

void KInterface::OnTextBox3ValueChanged(float delta)
{
	image1_->GetRect().AddOffset(glm::vec2(0.0f, delta / 450.0f));
}
