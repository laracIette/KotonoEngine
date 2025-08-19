#include "Interface.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ImageTextureManager.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Shader.h>
#include "Engine.h"
#include "InterfaceImageObject.h"
#include "InterfaceImageComponent.h"
#include "InterfaceTextBoxObject.h"
#include "InterfaceFloatTextBoxObject.h"
#include "InterfaceTextComponent.h"
#include "InterfaceWindowObject.h"
#include "ObjectManager.h"
#include "Timer.h"
#include <log.h>

void SInterface::Init()
{
	viewport_ = Engine.GetObjectManager().Create<RInterfaceWindowObject>("Viewport");

	image1_ = Engine.GetObjectManager().Create<RInterfaceImageObject>();
	auto* image2 = Engine.GetObjectManager().Create<RInterfaceImageObject>();
	auto* updateTextBox = Engine.GetObjectManager().Create<RInterfaceTextBoxObject>();
	auto* drawTextBox = Engine.GetObjectManager().Create<RInterfaceTextBoxObject>();
	//auto* textBox2 = Engine.GetObjectManager().Create<RInterfaceFloatTextBoxObject>();
	//auto* textBox3 = Engine.GetObjectManager().Create<RInterfaceFloatTextBoxObject>();
	image1_->GetImageComponent()->SetScreenSize(glm::vec2(1024.0f, 1024.0f));
	image2->GetImageComponent()->SetScreenSize(glm::vec2(1024.0f, 1024.0f));

	auto* shader2D = Framework.GetShaderManager().Get(Framework.GetPath().GetFrameworkPath() / R"(shaders\shader2D.ktshader)");
	shader2D->SetName("2D Shader");

	auto* imageTexture1 = Framework.GetImageTextureManager().Get(Framework.GetPath().GetSolutionPath() / R"(assets\models\viking_room.png)");
	auto* imageTexture2 = Framework.GetImageTextureManager().Get(Framework.GetPath().GetSolutionPath() / R"(assets\textures\default_texture.jpg)");

#if true
	image1_->GetRootComponent()->SetScreenSize(glm::vec2(1024.0f, 1024.0f));
	image1_->GetRootComponent()->SetRelativeScale(glm::vec2(0.25f));
	image1_->GetImageComponent()->SetShader(shader2D);
	image1_->GetImageComponent()->SetImageTexture(imageTexture1);
	//image1->GetRootComponent()->SetAnchor(EAnchor::TopLeft);

	image2->GetRootComponent()->SetScreenSize(glm::vec2(1024.0f, 1024.0f));
	image2->GetRootComponent()->SetRelativeScale(glm::vec2(0.10f));
	image2->GetImageComponent()->SetShader(shader2D);
	image2->GetImageComponent()->SetImageTexture(imageTexture2);
#if true
	image2->SetParent(image1_, ECoordinateSpace::World);
#else
	image2->SetLayer(1);

	auto* interfaceObjectStack = Engine.GetObjectManager().Create<KInterfaceStackComponent>();
	interfaceObjectStack->SetOrientation(EOrientation::Horizontal);
	interfaceObjectStack->SetItemSpacing(0.1f);
	interfaceObjectStack->AddItem(image1);
	interfaceObjectStack->AddItem(image2);
#endif
#endif

	updateTextBox->GetRootComponent()->SetRelativePosition({ 0.4f, -0.85f });
	updateTextBox->GetTextComponent()->SetFontSize(32.0f);
	updateTextBox->GetTextComponent()->SetSpacing(0.04f);
	updateTextBox->GetTextComponent()->SetTextBinding([]() { return std::format("U {:.8f}s", Engine.GetObjectManager().GetAverageUpdateTime()); });
	
	drawTextBox->GetRootComponent()->SetRelativePosition({ 0.4f, -0.7f });
	drawTextBox->GetTextComponent()->SetFontSize(32.0f);
	drawTextBox->GetTextComponent()->SetSpacing(0.04f);
	drawTextBox->GetTextComponent()->SetTextBinding([]() { return std::format("D {:.8f}s", Engine.GetObjectManager().GetAverageDrawTime()); });

	auto* updateTextTimer = Engine.GetObjectManager().Create<KTimer>();
	updateTextTimer->SetDuration(UDuration::FromSeconds(1.0f / 60.0f));
	updateTextTimer->SetIsRepeat(true);
	updateTextTimer->GetEventCompleted().AddListener(KtDelegate(updateTextBox->GetTextComponent(), &KInterfaceTextComponent::UpdateTextWithBinding));
	updateTextTimer->GetEventCompleted().AddListener(KtDelegate(drawTextBox->GetTextComponent(), &KInterfaceTextComponent::UpdateTextWithBinding));
	updateTextTimer->Start();

	//textBox2->GetRootComponent()->SetRelativePosition(glm::vec2(0.3f, 0.3f));
	//textBox2->GetTextComponent()->SetFontSize(32.0f);
	//textBox2->GetTextComponent()->SetSpacing(0.04f);
	//textBox2->GetTextComponent()->SetTextBinding([this]() { return std::to_string(image1_->GetRootComponent()->GetScreenPosition().x); });
	//textBox2->GetEventValueChanged().AddListener(KtDelegate(this, &SInterface::OnTextBox2ValueChanged));

	//textBox3->GetRootComponent()->SetRelativePosition(glm::vec2(0.3f, 0.6f));
	//textBox3->GetTextComponent()->SetFontSize(32.0f);
	//textBox3->GetTextComponent()->SetSpacing(0.04f);
	//textBox3->GetTextComponent()->SetTextBinding([this]() { return std::to_string(image1_->GetRootComponent()->GetScreenPosition().y); });
	//textBox3->GetEventValueChanged().AddListener(KtDelegate(this, &SInterface::OnTextBox3ValueChanged));
}

void SInterface::OnTextBox2ValueChanged(const float delta) const
{
	image1_->GetRootComponent()->Translate({ delta / 800.0f, 0.0f });
}

void SInterface::OnTextBox3ValueChanged(const float delta) const
{
	image1_->GetRootComponent()->Translate({ 0.0f, delta / 450.0f });
}
