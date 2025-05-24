#include "InterfaceTextBoxObject.h"
#include "InterfaceTextComponent.h"
#include "InterfaceColliderComponent.h"

void RInterfaceTextBoxObject::Construct()
{
	Base::Construct();

	textComponent_ = AddComponent<KInterfaceTextComponent>();
	colliderComponent_ = AddComponent<KInterfaceColliderComponent>();
}

void RInterfaceTextBoxObject::Init()
{
	Base::Init();

	colliderComponent_->GetRect().SetParent(&textComponent_->GetRect(), ECoordinateSpace::Relative);

	ListenEvent(colliderComponent_->GetEventDown(), &RInterfaceTextBoxObject::OnColliderDown);
}

void RInterfaceTextBoxObject::Update()
{
	Base::Update();

	colliderComponent_->GetRect().SetRelativeSize(textComponent_->GetRect().GetRelativeSize());
}

KInterfaceTextComponent* RInterfaceTextBoxObject::GetTextComponent() const
{
	return textComponent_;
}

void RInterfaceTextBoxObject::OnColliderDown()
{
	KT_DEBUG_LOG(KT_LOG_COMPILE_TIME_LEVEL, "text box");
}
