#include "InterfaceBoxComponent.h"
#include "InterfaceObject.h"
#include <kotono_graphics/InterfaceProxy.h>
#include <kotono_graphics/Texture.h>
#include <kotono_common/Path.h>
#include <kotono_graphics/Renderer.h>
#include <kotono_graphics/ShaderManager.h>
#include <kotono_graphics/TextureManager.h>
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

    EventColorChanged().AddListener(KtDelegate(this, &KInterfaceBoxComponent::MarkBoxProxyColorDirty));
    EventRectChanged().AddListener(KtDelegate(this, &KInterfaceBoxComponent::MarkBoxProxyRectDirty));
}

void KInterfaceBoxComponent::CreateBoxProxy()
{
    boxProxy_->ScheduleUpdate(
        [this](UInterfaceProxy::Data& data)
        {
            static const auto shaderPath{ KtPath::Graphics() / "shaders" / "flatColor2D.ktshader" };
            static const auto texturePath{ KtPath::Graphics() / "assets" / "textures" / "white_texture.jpg" };

            data.shader = ShaderManager.Get(shaderPath);
            data.renderable = TextureManager.Get(texturePath);
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
