#include "InterfaceComponent.h"
#include "InterfaceObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <kotono_common/log.h>
//#include <kotono_common/PathManager.h>
//#include <kotono_graphics/Texture.h>
//#include <kotono_graphics/TextureManager.h>
//#include <kotono_graphics/InterfaceProxy.h>
//#include <kotono_graphics/Renderer.h>
//#include <kotono_graphics/Shader.h>
//#include <kotono_graphics/ShaderManager.h>
#include <kotono_input/Mouse.h>
#include <kotono_platform/WindowViewport.h>

KInterfaceComponent::KInterfaceComponent(UPtrOwnerBase* ptrOwner) :
    Base(ptrOwner),
    visibility_(EVisibility::Visible),
    modelMatrix_([this]() { return TranslationMatrix() * RotationMatrix() * ScaleMatrix(); }),
    color_(UColor::White())
{
    eventRectChanged_.AddListener(KtDelegate(&modelMatrix_, &KtCached<glm::mat4>::MarkDirty));

    //boundsProxy_ = Renderer.InterfaceRenderer().CreateProxy();
}

void KInterfaceComponent::Cleanup()
{
    //Renderer.InterfaceRenderer().UnregisterProxy(boundsProxy_);
    //Renderer.InterfaceRenderer().DeleteProxy(boundsProxy_);

    owner_->GetViewport()->EventExtentChanged()
        .RemoveListener(KtDelegate(&eventRectChanged_, &KtEvent<>::Broadcast));

    SetParent(nullptr, ECoordinateSpace::Relative);
    SetOwner(nullptr);

    Base::Cleanup();
}

void KInterfaceComponent::Init()
{
}

void KInterfaceComponent::Update(const float deltaTime)
{
}

const UPtr<RInterfaceObject>& KInterfaceComponent::GetOwner() const
{
    return owner_;
}

const UPtr<KInterfaceComponent>& KInterfaceComponent::GetParent() const
{
    return parent_;
}

bool KInterfaceComponent::GetCanUpdate() const
{
    return canUpdate_;
}

const URect& KInterfaceComponent::GetRect() const
{
    return rect_;
}

EVisibility KInterfaceComponent::GetVisibility() const
{
    return visibility_;
}

i32 KInterfaceComponent::GetLayer() const
{
    if (parent_)
    {
        return parent_->GetLayer() + rect_.layer + 1;
    }
    return rect_.layer;
}

KtEvent<>& KInterfaceComponent::EventRectChanged()
{
    return eventRectChanged_;
}

KtEvent<>& KInterfaceComponent::EventLayerChanged()
{
    return eventLayerChanged_;
}

KtEvent<>& KInterfaceComponent::EventColorChanged()
{
    return eventColorChanged_;
}

const glm::vec2& KInterfaceComponent::GetRelativePosition() const
{
    return rect_.position;
}

const glm::vec2& KInterfaceComponent::GetRelativeScale() const
{
    return rect_.scale;
}

float KInterfaceComponent::GetRelativeRotation(const ERotationUnit unit) const
{
    return rect_.rotation;
}

glm::vec2 KInterfaceComponent::GetWorldPosition() const
{
    if (parent_)
    {
        return rect_.position + parent_->GetWorldPosition();
    }
    return rect_.position;
}

glm::vec2 KInterfaceComponent::GetWorldScale() const
{
    if (parent_)
    {
        return rect_.scale * parent_->GetWorldScale();
    }
    return rect_.scale;
}

float KInterfaceComponent::GetWorldRotation(const ERotationUnit unit) const
{
    if (parent_)
    {
        return GetRelativeRotation(unit) + parent_->GetWorldRotation(unit);
    }
    return GetRelativeRotation(unit);
}

EAnchor KInterfaceComponent::GetAnchor() const
{
    return rect_.anchor;
}

const UColor& KInterfaceComponent::GetColor() const
{
    return color_;
}

void KInterfaceComponent::SetOwner(const UPtr<RInterfaceObject>& owner)
{
    if (owner == owner_)
    {
        return;
    }

    if (owner_)
    {
        owner_->RemoveComponent(Ptr());
    }

    owner_ = owner;

    if (!owner_)
    {
        return;
    }

    owner_->AddComponent(Ptr());
}

void KInterfaceComponent::SetCanUpdate(const bool canUpdate)
{
	canUpdate_ = canUpdate;
}

void KInterfaceComponent::SetVisibility(const EVisibility visibility, const bool propagateToChildren)
{
    visibility_ = visibility;
    if (propagateToChildren)
    {
        for (auto& interfaceComponent : children_)
        {
            interfaceComponent->SetVisibility(visibility, propagateToChildren);
        }
    }
}

void KInterfaceComponent::SetLayer(const i32 layer)
{
    rect_.layer = layer;
    eventLayerChanged_.Broadcast();
}

void KInterfaceComponent::SetParent(const UPtr<KInterfaceComponent>& parent, const ECoordinateSpace keepRect)
{
    if (parent_ == parent)
    {
        return;
    }

    if (parent_)
    {
        parent_->EventRectChanged().RemoveListener(KtDelegate(&eventRectChanged_, &KtEvent<>::Broadcast));
        parent_->RemoveChildren(Ptr());
    }

    switch (keepRect)
    {
    case ECoordinateSpace::Relative:
    {
        parent_ = parent;
        eventRectChanged_.Broadcast();
        break;
    }
    case ECoordinateSpace::World:
    {
        //const auto size{ GetWorldSize() };
        const auto position{ GetWorldPosition() };
        const auto scale{ GetWorldScale() };
        const auto rotation{ GetWorldRotation() };
        parent_ = parent;
        //SetWorldSize(size);
        SetWorldPosition(position);
        SetWorldScale(scale);
        SetWorldRotation(rotation);
        break;
    }
    }

    if (parent_)
    {
        parent_->EventRectChanged().AddListener(KtDelegate(&eventRectChanged_, &KtEvent<>::Broadcast));
        parent_->AddChildren(Ptr());
    }
}

void KInterfaceComponent::SetRelativePosition(const glm::vec2& relativePosition)
{
    if (rect_.position == relativePosition)
    {
        return;
    }

    rect_.position = relativePosition;
    eventRectChanged_.Broadcast();
}

void KInterfaceComponent::SetRelativeScale(const glm::vec2& relativeScale)
{
    if (rect_.scale == relativeScale)
    {
        return;
    }

    rect_.scale = relativeScale;
    eventRectChanged_.Broadcast();
}

void KInterfaceComponent::SetRelativeRotation(float relativeRotation, const ERotationUnit unit)
{
    if (unit == ERotationUnit::Degrees)
    {
        relativeRotation = glm::radians(relativeRotation);
    }

    if (rect_.rotation == relativeRotation)
    {
        return;
    }

    const glm::vec2 translated = GetAnchorOffset();

    const float cosA = cos(relativeRotation);
    const float sinA = sin(relativeRotation);
    const auto rotated = glm::vec2(
        translated.x * cosA - translated.y * sinA,
        translated.x * sinA + translated.y * cosA
    );

    rect_.position = rotated + GetAnchorRelativePosition();
    rect_.rotation = relativeRotation;
    eventRectChanged_.Broadcast();
}

//void KInterfaceComponent::SetWorldSize(const glm::vec2& worldSize)
//{
//    if (parent_)
//    {
//        SetRelativeSize(worldSize / parent_->GetWorldScale());
//        return;
//    }
//    SetRelativeSize(worldSize);
//}

void KInterfaceComponent::SetWorldPosition(const glm::vec2& worldPosition)
{
    if (parent_)
    {
        SetRelativePosition(worldPosition - parent_->GetWorldPosition());
        return;
    }
    SetRelativePosition(worldPosition);
}

void KInterfaceComponent::SetWorldScale(const glm::vec2& worldScale)
{
    if (parent_)
    {
        SetRelativeScale(worldScale / parent_->GetWorldScale());
        return;
    }
    SetRelativeScale(worldScale);
}

void KInterfaceComponent::SetWorldRotation(const float worldRotation, const ERotationUnit unit)
{
    if (parent_)
    {
        SetRelativeRotation(worldRotation - parent_->GetWorldRotation(unit), unit);
        return;
    }
    SetRelativeRotation(worldRotation, unit);
}

void KInterfaceComponent::Translate(const glm::vec2& offset)
{
    SetRelativePosition(GetRelativePosition() + offset);
}

void KInterfaceComponent::Scale(const glm::vec2& scale)
{
    SetRelativeScale(GetRelativeScale() * scale);
}

void KInterfaceComponent::Rotate(const float rotation, const ERotationUnit unit)
{
    SetRelativeRotation(GetRelativeRotation(unit) + rotation, unit);
}

void KInterfaceComponent::SetAnchor(const EAnchor anchor)
{
    const auto worldPosition = GetWorldPosition();
    rect_.anchor = anchor;
    SetWorldPosition(worldPosition);

}

void KInterfaceComponent::SetColor(const UColor& color)
{
    color_ = color;
    eventColorChanged_.Broadcast();
}

glm::mat4 KInterfaceComponent::TranslationMatrix() const
{
    return glm::translate(glm::identity<glm::mat4>(), glm::vec3(GetWorldPositionWithAnchorOffset(), 0.0f));
}

glm::mat4 KInterfaceComponent::RotationMatrix() const
{
    return glm::rotate(glm::identity<glm::mat4>(), GetWorldRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::mat4 KInterfaceComponent::ScaleMatrix() const
{
    //const auto viewportSize = glm::vec2(GetOwner()->GetViewport()->GetExtent());
    //const float aspectRatio = GetOwner()->GetViewport()->GetAspectRatio();
    //const float rotation = GetWorldRotation(ERotationUnit::Radians);

    //// x *= 1 at rot 0
    //// y *= 1 at rot 0
    //// x *= aspectRatio at rot pi/2
    //// y *= 1 / aspectRatio at rot pi/2

    //const float value = (cos((rotation + glm::half_pi<float>()) * 2.0f) + 1.0f) / 2.0f;
    //const auto stretchCorrection = glm::vec2(std::lerp(1.0f, aspectRatio, value), std::lerp(1.0f, 1.0f / aspectRatio, value));

    //const auto size = GetScreenSize() / viewportSize * 2.0f * stretchCorrection;

    //return glm::scale(glm::identity<glm::mat4>(), glm::vec3(size, 1.0f));

    const auto worldScale{ GetWorldScale() };
    const auto viewportSize{ glm::vec2(GetOwner()->GetViewport()->GetExtent()) };
    const auto scale{ worldScale / viewportSize * viewportSize.y };
 
    return glm::scale(glm::identity<glm::mat4>(), glm::vec3(scale, 1.0f));
}

glm::mat4 KInterfaceComponent::ModelMatrix()
{
    return modelMatrix_;
}

glm::vec2 KInterfaceComponent::GetDirection(const UPtr<KInterfaceComponent>& target) const
{
    return target->GetWorldPosition() - GetWorldPosition();
}

float KInterfaceComponent::GetDistance(const UPtr<KInterfaceComponent>& other) const
{
    return glm::distance(GetWorldPosition(), other->GetWorldPosition());
}

bool KInterfaceComponent::GetIsOverlapping(const glm::vec2& worldPosition, const glm::vec2& worldScale) const
{
    const auto distance = glm::abs(GetWorldPosition() - worldPosition);
    const auto maxDistance = (GetWorldScale() + worldScale) / 2.0f;
    return distance.x < maxDistance.x
        && distance.y < maxDistance.y;
}

bool KInterfaceComponent::GetIsOverlapping(const glm::vec2& worldPosition) const
{
    return GetIsOverlapping(worldPosition, glm::vec2(0.0f));
}

bool KInterfaceComponent::GetIsOverlapping(const UPtr<KInterfaceComponent>& other) const
{
    return GetIsOverlapping(other->GetWorldPosition(), other->GetWorldScale());
}

bool KInterfaceComponent::IsHovered() const
{
    const auto& cursorPosition{ Mouse.CursorPosition() };
    const auto viewportSize{ glm::vec2(WindowViewport.GetExtent()) };
    const auto worldPosition{ cursorPosition / viewportSize * 2.0f - 1.0f };
    return GetIsOverlapping(worldPosition);
}

void KInterfaceComponent::AddChildren(const UPtr<KInterfaceComponent>& interfaceComponent)
{
    if (!interfaceComponent)
    {
        return;
    }

    children_.Add(interfaceComponent);
    interfaceComponent->childrenIndex_ = static_cast<size>(children_.LastIndex());
}

void KInterfaceComponent::RemoveChildren(const UPtr<KInterfaceComponent>& interfaceComponent)
{
    if (!interfaceComponent)
    {
        return;
    }

    const size index{ interfaceComponent->childrenIndex_ };
    if (children_.RemoveAt(index) == KtPoolRemoveResult::ItemSwappedAndRemoved)
    {
        children_[index]->childrenIndex_ = index;
    }
}

void KInterfaceComponent::Spawn()
{
    //CreateBoundsProxy();
    //Renderer.InterfaceRenderer().RegisterProxy(boundsProxy_);

    //eventRectChanged_.AddListener(KtDelegate(this, &Self::MarkBoundsProxyRectDirty));

    owner_->GetViewport()->EventExtentChanged()
        .AddListener(KtDelegate(&eventRectChanged_, &KtEvent<>::Broadcast));
}

//void KInterfaceComponent::CreateBoundsProxy()
//{
//    boundsProxy_->ScheduleUpdate(
//        [this](UInterfaceProxy::Data& data)
//        {
//            static const auto shaderPath{ PathManager.Graphics() / "shaders" / "flatColor2D.ktshader" };
//            static const auto texturePath{ PathManager.Graphics() / "assets" / "textures" / "white_texture.jpg" };
//
//            data.shader = ShaderManager.Get(shaderPath);
//            data.renderable = TextureManager.Get(texturePath);
//            data.layer = GetLayer();
//            data.objectData.modelMatrix = ModelMatrix();
//            data.objectData.color = { 1.0f, 1.0f, 1.0f, 0.01f };
//            data.scissor.offset = WindowViewport.GetOffset();
//            data.scissor.extent = WindowViewport.GetExtent();
//        }
//    );
//}

//void KInterfaceComponent::MarkBoundsProxyRectDirty()
//{
//    boundsProxy_->ScheduleUpdate(
//        [this](UInterfaceProxy::Data& data)
//        {
//            data.objectData.modelMatrix = ModelMatrix();
//        }
//    );
//}

glm::vec2 KInterfaceComponent::GetAnchorOffset() const
{
    return {
        (GetAnchor() & EAnchor::Left) == EAnchor::Left
            ? GetRelativeScale().x / 2.0f
            : (GetAnchor() & EAnchor::Right) == EAnchor::Right
                ? -GetRelativeScale().x / 2.0f
                : 0.0f,
        (GetAnchor() & EAnchor::Top) == EAnchor::Top
            ? GetRelativeScale().y / 2.0f
            : (GetAnchor() & EAnchor::Bottom) == EAnchor::Bottom
                ? -GetRelativeScale().y / 2.0f
                : 0.0f
    };
}

glm::vec2 KInterfaceComponent::GetWorldPositionWithAnchorOffset() const
{
    if (parent_)
    {
        return GetRelativePosition() + GetAnchorOffset() + parent_->GetWorldPositionWithAnchorOffset();
    }
    return GetRelativePosition() + GetAnchorOffset();
}

glm::vec2 KInterfaceComponent::GetAnchorRelativePosition() const
{
    return GetRelativePosition() - GetAnchorOffset();
}
