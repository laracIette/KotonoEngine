#include "InterfaceBoxComponent.h"
#include "InterfaceObject.h"
#include <kotono_common/AssetManager.h>
#include <kotono_graphics/InterfaceProxy.h>
#include <kotono_graphics/Texture.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/ShaderManager.h>
#include <kotono_platform/WindowViewport.h>

KInterfaceBoxComponent::KInterfaceBoxComponent(UPtrOwnerBase* ptrOwner) 
    : Base(ptrOwner)
{
    boxProxy_ = Renderer.InterfaceRenderer().CreateProxy();
}

KInterfaceBoxComponent::~KInterfaceBoxComponent()
{
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

    EventColorChanged().AddListener(this, &KInterfaceBoxComponent::MarkBoxProxyColorDirty);
    EventRectChanged().AddListener(this, &KInterfaceBoxComponent::MarkBoxProxyRectDirty);
}

void KInterfaceBoxComponent::CreateBoxProxy()
{
    boxProxy_->ScheduleUpdate(
        [this](UInterfaceProxy::Data& data)
        {
            data.shader = ShaderManager.Get("${ENGINE_DIRECTORY}/Graphics/shaders/flatColor2D.ktshader");
            data.renderable = UAssetManager<KtTexture>::Get("${ENGINE_DIRECTORY}/Graphics/assets/textures/white_texture.jpg");
            data.layer = GetLayer();
            data.objectData.modelMatrix = ModelMatrix();
            data.objectData.color = GetColor();
            data.scissor.offset = WindowViewport.GetOffset();
            data.scissor.extent = WindowViewport.GetExtent();
        }
    );
}

void KInterfaceBoxComponent::MarkBoxProxyColorDirty()
{
    boxProxy_->ScheduleUpdate(
        [this](UInterfaceProxy::Data& data)
        {
            data.objectData.color = GetColor();
        }
    );
}

void KInterfaceBoxComponent::MarkBoxProxyRectDirty()
{
    boxProxy_->ScheduleUpdate(
        [this](UInterfaceProxy::Data& data)
        {
            data.objectData.modelMatrix = ModelMatrix();
        }
    );
}
