#include "Image.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/Renderer.h>
#include "Engine.h"
#include "ObjectManager.h"
#include "Time.h"

void RImage::Init()
{
	Base::Init();
	_spinTask = Engine.GetObjectManager().Create<OTask>();
	_spinTask->GetEventUpdate().AddListener(this, &RImage::Spin);
	_spinTask->SetDuration(10.0f);
	Framework.GetInputManager().GetKeyboard()
		.GetEvent(KT_KEY_SPACE, KT_INPUT_STATE_PRESSED)
		.AddListener(_spinTask, &OTask::Start);
}

void RImage::Update()
{
	Base::Update();
	AddToRenderQueue();
}

void RImage::Cleanup()
{
	Base::Cleanup();
	_spinTask->SetIsDelete(true);
	Framework.GetInputManager().GetKeyboard()
		.GetEvent(KT_KEY_SPACE, KT_INPUT_STATE_PRESSED)
		.RemoveListener(this);
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
	Framework.GetRenderer().GetRenderer2D().AddToRenderQueue(args);
}

void RImage::Spin()
{
	GetRect().AddRotation(Engine.GetTime().GetDelta());
}
