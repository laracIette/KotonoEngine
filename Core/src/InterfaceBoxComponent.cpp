#include "InterfaceBoxComponent.h"
#include "InterfaceObject.h"
#include <kotono_graphics/InterfaceRenderableProxy.h>
#include <kotono_graphics/ImageTexture.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/ShaderManager.h>
#include <kotono_graphics/ImageTextureManager.h>
#include <kotono_platform/WindowViewport.h>

KInterfaceBoxComponent::KInterfaceBoxComponent(UPtrOwnerBase* ptrOwner) :
	Base(ptrOwner)
{
    boxProxy_ = Renderer.InterfaceRenderer().CreateProxy();
}

void KInterfaceBoxComponent::Cleanup()
{
    Base::Cleanup();

    Renderer.InterfaceRenderer().Unregister(boxProxy_);
    Renderer.InterfaceRenderer().DeleteProxy(boxProxy_);
}

void KInterfaceBoxComponent::Init()
{
    Base::Init();

    CreateBoxProxy();
    Renderer.InterfaceRenderer().Register(boxProxy_);

    EventColorUpdated().AddListener(KtDelegate(this, &KInterfaceBoxComponent::MarkBoxProxyColorDirty));
    EventRectUpdated().AddListener(KtDelegate(this, &KInterfaceBoxComponent::MarkBoxProxyRectDirty));
}

void KInterfaceBoxComponent::CreateBoxProxy()
{
    const auto shaderPath{ KtPath::Graphics() / R"(shaders\flatColor2D.ktshader)" };
    const auto texturePath{ KtPath::Graphics() / R"(assets\textures\white_texture.jpg)" };
    
    boxProxy_->shader = ShaderManager.Get(shaderPath);
    boxProxy_->renderable = ImageTextureManager.Get(texturePath);
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
