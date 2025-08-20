#include "InterfaceBoxComponent.h"
#include "InterfaceObject.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Renderable2DProxy.h>
#include <kotono_framework/ImageTexture.h>

KInterfaceBoxComponent::KInterfaceBoxComponent(RInterfaceObject* owner) :
	Base(owner)
{
    boxProxy_ = Framework.GetRenderer().GetInterfaceRenderer().CreateProxy();
}

void KInterfaceBoxComponent::Init()
{
    Base::Init();

    CreateBoxProxy();
    Framework.GetRenderer().GetInterfaceRenderer().Register(boxProxy_);

    GetEventColorUpdated().AddListener(KtDelegate(this, &KInterfaceBoxComponent::MarkBoxProxyColorDirty));
    GetEventRectUpdated().AddListener(KtDelegate(this, &KInterfaceBoxComponent::MarkBoxProxyRectDirty));
}

void KInterfaceBoxComponent::Cleanup()
{
    Base::Cleanup();

    Framework.GetRenderer().GetInterfaceRenderer().Unregister(boxProxy_);
    Framework.GetRenderer().GetInterfaceRenderer().DeleteProxy(boxProxy_);
}

void KInterfaceBoxComponent::CreateBoxProxy()
{
    const auto shaderPath = Framework.GetPath().GetFrameworkPath() / R"(shaders\flatColor2D.ktshader)";
    const auto texturePath = Framework.GetPath().GetSolutionPath() / R"(assets\textures\white_texture.jpg)";
    
    boxProxy_->shader = Framework.GetShaderManager().Get(shaderPath);
    boxProxy_->viewport = GetOwner()->GetViewport();
    boxProxy_->renderable = Framework.GetImageTextureManager().Get(texturePath);
    boxProxy_->layer = GetLayer();
    boxProxy_->objectData.modelMatrix = GetModelMatrix();
    boxProxy_->objectData.color = GetColor();
#if _DEBUG
    boxProxy_->source = this;
#endif
}

void KInterfaceBoxComponent::MarkBoxProxyColorDirty()
{
    boxProxy_->isDirty = true;
    boxProxy_->objectData.color = GetColor();
}

void KInterfaceBoxComponent::MarkBoxProxyRectDirty()
{
    boxProxy_->isDirty = true;
    boxProxy_->objectData.modelMatrix = GetModelMatrix();
}
