#include "InterfaceObject.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Window.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/InputManager.h>
#include <kotono_framework/ImageTextureManager.h>
#include <kotono_framework/log.h>
#include <kotono_framework/Shader.h>
#include <kotono_framework/Viewport.h>
#include <kotono_framework/AddToRenderQueue2DArgs.h>
#include "Engine.h"
#include "InterfaceObjectComponent.h"
#include "ObjectManager.h"
#include "InterfaceObjectColliderComponent.h"

static KtShader* FlatColorShader = nullptr;
static KtImageTexture* FlatColorTexture = nullptr;

void RInterfaceObject::Construct()
{
	Base::Construct();

	collider_ = Engine.GetObjectManager().Create<KInterfaceObjectColliderComponent>();
	collider_->SetOwner(this);
	components_.insert(collider_);

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

	collider_->GetRect().SetRelativeSize(rect_.GetRelativeSize());
	
	ListenEvent(Engine.GetObjectManager().GetEventDrawInterfaceObjectBounds(), &RInterfaceObject::AddBoundsToRenderQueue);
	ListenEvent(collider_->GetEventDown(), &RInterfaceObject::OnEventColliderMouseLeftButtonDown);
}

void RInterfaceObject::Update()
{
	Base::Update();
}

void RInterfaceObject::Cleanup()
{
	Base::Cleanup();

	//collider_->SetIsDelete(true);
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

RInterfaceObject* RInterfaceObject::GetRoot()
{
	if (parent_)
	{
		return parent_->GetRoot();
	}
	return this;
}

const int32_t RInterfaceObject::GetLayer() const
{
	if (parent_)
	{
		return layer_ + parent_->GetLayer() + 1;
	}
	return layer_;
}

const std::unordered_set<RInterfaceObject*>& RInterfaceObject::GetChildren() const
{
	return children_;
}

const bool RInterfaceObject::GetIsSizeToContent() const
{
	return isSizeToContent_;
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
	if (parent)
	{
		parent->children_.insert(this);
	}
	if (parent_)
	{
		parent_->children_.erase(this);
	}
	parent_ = parent;
	rect_.SetParent(parent_ ? &parent_->rect_ : nullptr, keepRect);
}

void RInterfaceObject::SetLayer(const int32_t layer)
{
	layer_ = layer;
}

void RInterfaceObject::GetIsSizeToContent(const bool isSizeToContent)
{
	isSizeToContent_ = isSizeToContent;
}

void RInterfaceObject::AddBoundsToRenderQueue()
{
	KtAddToRenderQueue2DArgs args{};
	args.Shader = FlatColorShader;
	args.Renderable = FlatColorTexture;
	args.Viewport = GetViewport();
	args.ObjectData.Model = rect_.GetModelMatrix();
	args.Layer = GetLayer();
	Framework.GetRenderer().GetRenderer2D().AddToRenderQueue(args);
}

void RInterfaceObject::OnEventColliderMouseLeftButtonDown()
{
	const auto& windowSize = Framework.GetWindow().GetSize();
	const auto cursorPositionDelta = Framework.GetInputManager().GetMouse().GetCursorPositionDelta();
	const auto cursorPositionDeltaNormalized = 2.0f * cursorPositionDelta / glm::vec2(windowSize);
	rect_.AddOffset(cursorPositionDeltaNormalized);
}