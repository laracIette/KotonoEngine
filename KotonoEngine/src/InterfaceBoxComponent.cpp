#include "InterfaceBoxComponent.h"
#include "InterfaceObject.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/InterfaceRenderableProxy.h>
#include <kotono_framework/ImageTexture.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ImageTextureManager.h>
#include <kotono_framework/WindowViewport.h>

KInterfaceBoxComponent::KInterfaceBoxComponent(UPtrOwnerBase* ptrOwner, const UPtr<RInterfaceObject>& owner) :
	Base(ptrOwner, owner)
{
    boxProxy_ = Framework.Renderer().GetInterfaceRenderer().CreateProxy();
}

void KInterfaceBoxComponent::Init()
{
    Base::Init();

    CreateBoxProxy();
    Framework.Renderer().GetInterfaceRenderer().Register(boxProxy_);

    EventColorUpdated().AddListener(KtDelegate(this, &KInterfaceBoxComponent::MarkBoxProxyColorDirty));
    EventRectUpdated().AddListener(KtDelegate(this, &KInterfaceBoxComponent::MarkBoxProxyRectDirty));
}

void KInterfaceBoxComponent::Cleanup()
{
    Base::Cleanup();

    Framework.Renderer().GetInterfaceRenderer().Unregister(boxProxy_);
    Framework.Renderer().GetInterfaceRenderer().DeleteProxy(boxProxy_);
}

void KInterfaceBoxComponent::CreateBoxProxy()
{
    const auto shaderPath = Framework.Path().Framework() / R"(shaders\flatColor2D.ktshader)";
    const auto texturePath = Framework.Path().Framework() / R"(assets\textures\white_texture.jpg)";
    
    boxProxy_->shader = Framework.ShaderManager().Get(shaderPath);
    boxProxy_->renderable = Framework.ImageTextureManager().Get(texturePath);
    boxProxy_->layer = GetLayer();
    boxProxy_->objectData.modelMatrix = ModelMatrix();
    boxProxy_->objectData.color = GetColor(); 
    boxProxy_->scissor.offset = WindowViewport.GetOffset();
    boxProxy_->scissor.extent = WindowViewport.GetExtent();
#   ifdef _DEBUG
        boxProxy_->source = this;
#   endif
}

void KInterfaceBoxComponent::MarkBoxProxyColorDirty()
{
    boxProxy_->isDirty = true;
    boxProxy_->objectData.color = GetColor();
}

void KInterfaceBoxComponent::MarkBoxProxyRectDirty()
{
    boxProxy_->isDirty = true;
    boxProxy_->objectData.modelMatrix = ModelMatrix();
}
