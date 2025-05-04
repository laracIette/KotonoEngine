#include "InterfaceObject.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ImageTextureManager.h>
#include <kotono_framework/log.h>
#include <kotono_framework/Shader.h>
#include <kotono_framework/AddToRenderQueue2DArgs.h>
#include "Engine.h"
#include "ObjectManager.h"
#include "Visualizer.h"

static KtShader* FlatColorShader = nullptr;
static KtImageTexture* FlatColorTexture = nullptr;

void RInterfaceObject::Init()
{
	Base::Init();

	_visibility = EVisibility::EditorAndGame;
	_viewport = &WindowViewport;

	ListenEvent(Engine.GetObjectManager().GetEventDrawInterfaceObjectBounds(), &RInterfaceObject::AddBoundsToRenderQueue);

	if (!FlatColorShader)
	{
		const auto path = Framework.GetPath().GetFrameworkPath() / R"(shaders\flatColor2D.ktshader)";
		FlatColorShader = Framework.GetShaderManager().Create(path);
		FlatColorShader->SetName("2D Flat Color Shader");
	}
	if (!FlatColorTexture)
	{
		const auto path = Framework.GetPath().GetSolutionPath() / R"(assets\textures\white_texture.jpg)";
		FlatColorTexture = Framework.GetImageTextureManager().Create(path);
	}
}

void RInterfaceObject::Update()
{
	Base::Update();
}

void RInterfaceObject::Cleanup()
{
	Base::Cleanup();
}

const URect& RInterfaceObject::GetRect() const
{
	return _rect;
}

URect& RInterfaceObject::GetRect() 
{
	return _rect;
}

KtViewport* RInterfaceObject::GetViewport() const
{
	return _viewport;
}

RInterfaceObject* RInterfaceObject::GetParent() const
{
	return _parent;
}

const int32_t RInterfaceObject::GetLayer() const
{
	if (_parent)
	{
		return _layer + _parent->GetLayer() + 1;
	}
	return _layer;
}

const EVisibility RInterfaceObject::GetVisibility() const
{
	return _visibility;
}

void RInterfaceObject::SetVisibility(const EVisibility visibility)
{
	_visibility = visibility;
}

void RInterfaceObject::SetViewport(KtViewport* viewport)
{
	_viewport = viewport;
}

void RInterfaceObject::SetParent(RInterfaceObject* parent, const ECoordinateSpace keepRect)
{
	if (parent == this)
	{
		KT_DEBUG_LOG("RInterfaceObject::SetParent(): couldn't set the parent of '%s' to itself", GetName().c_str());
		return;
	}
	if (parent == _parent)
	{
		KT_DEBUG_LOG("RInterfaceObject::SetParent(): couldn't set the parent of '%s' to the same", GetName().c_str());
		return;
	}
	_parent = parent;
	_rect.SetParent(_parent ? &_parent->_rect : nullptr, keepRect);
}

void RInterfaceObject::SetLayer(const int32_t layer)
{
	_layer = layer;
}

void RInterfaceObject::AddBoundsToRenderQueue() 
{
	KtAddToRenderQueue2DArgs args{};
	args.Shader = FlatColorShader;
	args.Renderable = FlatColorTexture;
	args.Viewport = GetViewport();
	args.ObjectData.Model = GetRect().GetModelMatrix();
	args.Layer = GetLayer();
	Framework.GetRenderer().GetRenderer2D().AddToRenderQueue(args);
}

