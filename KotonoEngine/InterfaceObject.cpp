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

void RInterfaceObject::Construct()
{
	Base::Construct();

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

void RInterfaceObject::Init()
{
	Base::Init();

	visibility_ = EVisibility::EditorAndGame;
	viewport_ = &WindowViewport;

	ListenEvent(Engine.GetObjectManager().GetEventDrawInterfaceObjectBounds(), &RInterfaceObject::AddBoundsToRenderQueue);
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
	return rect_;
}

URect& RInterfaceObject::GetRect() 
{
	return rect_;
}

KtViewport* RInterfaceObject::GetViewport() const
{
	return viewport_;
}

RInterfaceObject* RInterfaceObject::GetParent() const
{
	return parent_;
}

const int32_t RInterfaceObject::GetLayer() const
{
	if (parent_)
	{
		return layer_ + parent_->GetLayer() + 1;
	}
	return layer_;
}

const EVisibility RInterfaceObject::GetVisibility() const
{
	return visibility_;
}

void RInterfaceObject::SetVisibility(const EVisibility visibility)
{
	visibility_ = visibility;
}

void RInterfaceObject::SetViewport(KtViewport* viewport)
{
	viewport_ = viewport;
}

void RInterfaceObject::SetParent(RInterfaceObject* parent, const ECoordinateSpace keepRect)
{
	if (parent == this)
	{
		KT_DEBUG_LOG("RInterfaceObject::SetParent(): couldn't set the parent of '%s' to itself", GetName().c_str());
		return;
	}
	if (parent == parent_)
	{
		KT_DEBUG_LOG("RInterfaceObject::SetParent(): couldn't set the parent of '%s' to the same", GetName().c_str());
		return;
	}
	parent_ = parent;
	rect_.SetParent(parent_ ? &parent_->rect_ : nullptr, keepRect);
}

void RInterfaceObject::SetLayer(const int32_t layer)
{
	layer_ = layer;
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

