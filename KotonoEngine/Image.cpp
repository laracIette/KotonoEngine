#include "Image.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Window.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/InputManager.h>
#include "Engine.h"
#include "ObjectManager.h"
#include <kotono_framework/log.h>

void RImage::Init()
{
	Base::Init();
	_collider = Engine.GetObjectManager().Create<RInterfaceCollider>();
	_collider->GetRect().SetBaseSize(GetRect().GetBaseSize());
	_collider->SetParent(this, ECoordinateSpace::Relative);
	_collider->GetEventOverlap().AddListener(this, &RImage::OnEventOverlap);
	Framework.GetInputManager().GetMouse()
		.GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_DOWN)
		.AddListener(this, &RImage::OnEventMouseLeftButtonDown);
}

void RImage::Update()
{
	Base::Update();
	AddToRenderQueue();
}

void RImage::Cleanup()
{
	Base::Cleanup();
	_collider->GetEventOverlap().RemoveListener(this);
	Framework.GetInputManager().GetMouse()
		.GetButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_DOWN)
		.RemoveListener(this);
	_collider->SetIsDelete(true);
}

KtShader* RImage::GetShader() const
{
	return _shader;
}

void RImage::SetShader(KtShader* shader)
{
	_shader = shader;
}

void RImage::AddToRenderQueue() const
{
	KtAddToRenderQueue2DArgs args{};
	args.Shader = _shader;
	args.Viewport = GetViewport();
	args.ObjectData.Model = GetRect().GetModelMatrix();
	args.Layer = GetLayer();
	Framework.GetRenderer().GetRenderer2D().AddToRenderQueue(args);
}


void RImage::OnEventOverlap(RInterfaceCollider* other)
{
	//KT_DEBUG_LOG("'%s' overlapping '%s'", other->GetName().c_str(), GetName().c_str());
}

void RImage::OnEventMouseLeftButtonDown()
{
	const auto& cursorPosition = Framework.GetInputManager().GetMouse().GetCursorPosition();
	if (_collider->GetRect().GetIsOverlapping(cursorPosition))
	{
		const auto& windowSize = Framework.GetWindow().GetSize();
		const auto cursorPositionDelta = Framework.GetInputManager().GetMouse().GetCursorPositionDelta();
		const auto cursorPositionDeltaNormalized = 2.0f * cursorPositionDelta / glm::vec2(windowSize);
		GetRect().AddOffset(cursorPositionDeltaNormalized);
	}
}