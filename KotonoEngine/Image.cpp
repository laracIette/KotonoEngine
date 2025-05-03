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
#include <kotono_framework/log.h>

void RImage::Init()
{
	Base::Init();

	Engine.GetObjectManager().GetEventDrawObjects().AddListener(this, &RImage::Draw);

	_collider = Engine.GetObjectManager().Create<RInterfaceCollider>();
	_collider->GetRect().SetBaseSize(GetRect().GetBaseSize());
	_collider->SetParent(this, ECoordinateSpace::Relative);
	_collider->GetEventDown().AddListener(this, &RImage::OnEventColliderMouseLeftButtonDown);
}

void RImage::Update()
{
	Base::Update();
}

void RImage::Cleanup()
{
	Base::Cleanup();
	_collider->GetEventDown().RemoveListener(this);
	_collider->SetIsDelete(true);

	Engine.GetObjectManager().GetEventDrawObjects().RemoveListener(this);
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
    GetRect().SetBaseSize(_imageTexture ? _imageTexture->GetSize() : glm::uvec2(0));
}

void RImage::Draw()
{
	if (Engine.GetVisualizer().GetIsFieldVisible(EVisualizationField::InterfaceObject))
	{
		AddToRenderQueue();
	}
	if (Engine.GetVisualizer().GetIsFieldVisible(EVisualizationField::Wireframe))
	{
		// ?
	}
}

void RImage::AddToRenderQueue() const
{
	KtAddToRenderQueue2DArgs args{};
	args.Shader = _shader;
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
