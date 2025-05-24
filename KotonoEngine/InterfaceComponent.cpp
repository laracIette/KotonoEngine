#include "InterfaceComponent.h"
#include "InterfaceObject.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Shader.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ImageTexture.h>
#include <kotono_framework/ImageTextureManager.h>
#include <kotono_framework/AddToRenderQueue2DArgs.h>
#include "Engine.h"
#include "ObjectManager.h"

static KtShader* FlatColorShader = nullptr;
static KtImageTexture* FlatColorTexture = nullptr;

KInterfaceComponent::KInterfaceComponent(RInterfaceObject* owner)
    : owner_(owner)
{
}

void KInterfaceComponent::Construct()
{
    Base::Construct();

    if (!FlatColorShader)
    {
        const auto path = Framework.GetPath().GetFrameworkPath() / R"(shaders\flatColor2D.ktshader)";
        FlatColorShader = Framework.GetShaderManager().Get(path);
        FlatColorShader->SetName("2D Flat Color Shader");
    }
    if (!FlatColorTexture)
    {
        const auto path = Framework.GetPath().GetSolutionPath() / R"(assets\textures\white_texture.jpg)";
        FlatColorTexture = Framework.GetImageTextureManager().Get(path);
    }

    rect_.SetParent(&owner_->GetRect(), ECoordinateSpace::Relative);
}

void KInterfaceComponent::Init()
{
    Base::Init();

    visibility_ = EVisibility::EditorAndGame;

    ListenEvent(Engine.GetObjectManager().GetEventDrawInterfaceObjectBounds(), &KInterfaceComponent::AddBoundsToRenderQueue);
}

RInterfaceObject* KInterfaceComponent::GetOwner() const
{
    return owner_;
}

const URect& KInterfaceComponent::GetRect() const
{
    return rect_;
}

URect& KInterfaceComponent::GetRect() 
{
    return rect_;
}

const EVisibility KInterfaceComponent::GetVisibility() const
{
    return visibility_;
}

const int32_t KInterfaceComponent::GetLayer() const
{
    return owner_->GetLayer() + layer_ + 1;
}

void KInterfaceComponent::SetVisibility(const EVisibility visibility)
{
    visibility_ = visibility;
}

void KInterfaceComponent::SetLayer(const int32_t layer)
{
    layer_ = layer;
}

void KInterfaceComponent::AddBoundsToRenderQueue() 
{
    KtAddToRenderQueue2DArgs args{};
    args.Shader = FlatColorShader;
    args.Renderable = FlatColorTexture;
    args.Viewport = owner_->GetViewport();
    args.ObjectData.Model = rect_.GetModelMatrix();
    args.Layer = GetLayer();
    Framework.GetRenderer().GetRenderer2D().AddToRenderQueue(args);
}