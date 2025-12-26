#include "InterfaceBoxComponent.h"
#include "InterfaceObject.h"
#include <kotono_graphics/InterfaceProxy.h>
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

    Renderer.InterfaceRenderer().UnregisterProxy(boxProxy_);
    Renderer.InterfaceRenderer().DeleteProxy(boxProxy_);
}

void KInterfaceBoxComponent::Init()
{
    Base::Init();
}

void KInterfaceBoxComponent::Spawn()
{
    Base::Spawn();

    CreateBoxProxy();
    Renderer.InterfaceRenderer().RegisterProxy(boxProxy_);

    EventColorUpdated().AddListener(KtDelegate(this, &KInterfaceBoxComponent::MarkBoxProxyColorDirty));
    EventRectUpdated().AddListener(KtDelegate(this, &KInterfaceBoxComponent::MarkBoxProxyRectDirty));
}

void KInterfaceBoxComponent::CreateBoxProxy()
{
    boxProxy_->ScheduleUpdate(
        [this](UInterfaceProxy::FrameData& frameData)
        {
            const auto shaderPath{ KtPath::Graphics() / "shaders" / "flatColor2D.ktshader" };
            const auto texturePath{ KtPath::Graphics() / "assets" / "textures" / "white_texture.jpg" };

            frameData.shader = ShaderManager.Get(shaderPath);
            frameData.renderable = ImageTextureManager.Get(texturePath);
            frameData.layer = GetLayer();
            frameData.objectData.modelMatrix = ModelMatrix();
            frameData.objectData.color = GetColor();
            frameData.scissor.offset = WindowViewport.GetOffset();
            frameData.scissor.extent = WindowViewport.GetExtent();
        }
    );
}

void KInterfaceBoxComponent::MarkBoxProxyColorDirty()
{
    boxProxy_->ScheduleUpdate(
        [this](UInterfaceProxy::FrameData& frameData)
        {
            frameData.objectData.color = GetColor();
        }
    );
}

void KInterfaceBoxComponent::MarkBoxProxyRectDirty()
{
    boxProxy_->ScheduleUpdate(
        [this](UInterfaceProxy::FrameData& frameData)
        {
            frameData.objectData.modelMatrix = ModelMatrix();
        }
    );
}
