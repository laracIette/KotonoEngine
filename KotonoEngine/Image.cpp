#include "Image.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Window.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/InputManager.h>
#include "Engine.h"
#include "ObjectManager.h"
#include "Visualizer.h"

static KtShader* WireframeShader = nullptr;

void RImage::Init()
{
	Base::Init();

	_collider = Engine.GetObjectManager().Create<RInterfaceCollider>();
	_collider->GetRect().SetSize(GetRect().GetSize());
	_collider->SetParent(this, ECoordinateSpace::Relative);

	ListenEvent(_collider->GetEventDown(), &RImage::OnEventColliderMouseLeftButtonDown);
	ListenEvent(Engine.GetObjectManager().GetEventDrawInterfaceObjects(), &RImage::AddTextureToRenderQueue);
	ListenEvent(Engine.GetObjectManager().GetEventDrawInterfaceObjectWireframes(), &RImage::AddWireframeToRenderQueue);

	if (!WireframeShader)
	{
		const auto path = Framework.GetPath().GetFrameworkPath() / R"(shaders\wireframe2D.ktshader)";
		WireframeShader = Framework.GetShaderManager().Create(path);
		WireframeShader->SetName("2D Wireframe Shader");
	}
}

void RImage::Update()
{
	Base::Update();
}

void RImage::Cleanup()
{
	Base::Cleanup();

	_collider->SetIsDelete(true);
}

KtShader* RImage::GetShader() const
{
	return _shader;
}

KtImageTexture* RImage::GetImageTexture() const
{
	return _imageTexture;
}

void RImage::SetShader(KtShader* shader)
{
	_shader = shader;
}

void RImage::SetImageTexture(KtImageTexture* imageTexture)
{
	_imageTexture = imageTexture;
}

void RImage::AddTextureToRenderQueue()
{
	KtAddToRenderQueue2DArgs args{};
	args.Shader = _shader;
	args.Renderable = _imageTexture;
	args.Viewport = GetViewport();
	args.ObjectData.Model = GetRect().GetModelMatrix();
	args.Layer = GetLayer();
	Framework.GetRenderer().GetRenderer2D().AddToRenderQueue(args);
}

void RImage::AddWireframeToRenderQueue()
{
	KtAddToRenderQueue2DArgs args{};
	args.Shader = WireframeShader;
	args.Renderable = _imageTexture;
	args.Viewport = GetViewport();
	args.ObjectData.Model = GetRect().GetModelMatrix();
	args.Layer = GetLayer();
	Framework.GetRenderer().GetRenderer2D().AddToRenderQueue(args);
}

void RImage::OnEventColliderMouseLeftButtonDown()
{
	const auto& windowSize = Framework.GetWindow().GetSize();
	const auto cursorPositionDelta = Framework.GetInputManager().GetMouse().GetCursorPositionDelta();
	const auto cursorPositionDeltaNormalized = 2.0f * cursorPositionDelta / glm::vec2(windowSize);
	GetRect().AddOffset(cursorPositionDeltaNormalized);
}
