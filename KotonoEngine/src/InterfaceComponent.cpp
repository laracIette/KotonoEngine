#include "InterfaceComponent.h"
#include "InterfaceObject.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Shader.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ImageTexture.h>
#include <kotono_framework/ImageTextureManager.h>
#include <kotono_framework/WindowViewport.h>
#include <kotono_framework/InterfaceRenderableProxy.h>
#include <kotono_framework/InputManager.h>
#include "log.h"

KInterfaceComponent::KInterfaceComponent(UPtrOwnerBase* ptrOwner, const UPtr<RInterfaceObject>& owner) :
    Base(ptrOwner),
    owner_(owner),
    modelMatrix_([this]() { return GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix(); }),
    color_(KtColor::White())
{
    eventRectUpdated_.AddListener(KtDelegate(&modelMatrix_, &KtCached<glm::mat4>::MarkDirty));

    if (Owner()->GetRootComponent() != this)
    {
        SetParent(Owner()->GetRootComponent(), ECoordinateSpace::Relative);
    }

    boundsProxy_ = Framework.Renderer().GetInterfaceRenderer().CreateProxy();
}

void KInterfaceComponent::Init()
{
    Base::Init();

    visibility_ = EVisibility::EditorAndGame;

    CreateBoundsProxy();
    Framework.Renderer().GetInterfaceRenderer().Register(boundsProxy_);

    EventRectUpdated().AddListener(KtDelegate(this, &KInterfaceComponent::MarkBoundsProxyRectDirty));
}

void KInterfaceComponent::Cleanup()
{
    for (int64_t i{ children_.LastIndex() }; i >= 0; --i)
    {
        children_[i]->SetParent(nullptr, ECoordinateSpace::Relative);
    }
    children_.Clear();

    SetParent(nullptr, ECoordinateSpace::Relative);

    Owner()->RemoveComponent(Ptr<KInterfaceComponent>());

    Framework.Renderer().GetInterfaceRenderer().Unregister(boundsProxy_);
    Framework.Renderer().GetInterfaceRenderer().DeleteProxy(boundsProxy_);

    Base::Cleanup();
}

const UPtr<RInterfaceObject>& KInterfaceComponent::Owner() const
{
    return owner_;
}

const UPtr<KInterfaceComponent>& KInterfaceComponent::GetParent() const
{
    return parent_;
}

const URect& KInterfaceComponent::GetRect() const
{
    return rect_;
}

EVisibility KInterfaceComponent::GetVisibility() const
{
    return visibility_;
}

int32_t KInterfaceComponent::GetLayer() const
{
    if (parent_)
    {
        return parent_->GetLayer() + layer_ + 1;
    }
    return layer_;
}

KtEvent<>& KInterfaceComponent::EventRectUpdated()
{
    return eventRectUpdated_;
}

KtEvent<>& KInterfaceComponent::EventLayerUpdated()
{
    return eventLayerUpdated_;
}

KtEvent<>& KInterfaceComponent::EventColorUpdated()
{
    return eventColorUpdated_;
}

const glm::vec2& KInterfaceComponent::GetRelativeSize() const
{
    return rect_.size;
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

glm::vec2 KInterfaceComponent::GetWorldSize() const
{
    return rect_.size * GetWorldScale();
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

glm::vec2 KInterfaceComponent::GetScreenPosition() const
{
    const auto viewportSize = glm::vec2(owner_->GetViewport()->GetExtent());
    const auto newPosition = (GetWorldPosition() + glm::vec2(1.0f)) * viewportSize / 2.0f;
    return newPosition;
}

glm::vec2 KInterfaceComponent::GetScreenSize() const
{
    const auto viewportSize = glm::vec2(owner_->GetViewport()->GetExtent());
    const auto newSize = GetWorldSize() * viewportSize / 2.0f;
    return newSize;
}

EAnchor KInterfaceComponent::GetAnchor() const
{
    return rect_.anchor;
}

const KtColor& KInterfaceComponent::GetColor() const
{
    return color_;
}

void KInterfaceComponent::SetVisibility(const EVisibility visibility)
{
    visibility_ = visibility;
}

void KInterfaceComponent::SetLayer(const int32_t layer)
{
    layer_ = layer;
    eventLayerUpdated_.Broadcast();
}

void KInterfaceComponent::SetParent(const UPtr<KInterfaceComponent>& parent, const ECoordinateSpace keepRect)
{
    if (parent_)
    {
        parent_->EventRectUpdated().RemoveListener(KtDelegate(&eventRectUpdated_, &KtEvent<>::Broadcast));
        parent_->RemoveChildren(Ptr<KInterfaceComponent>());
    }

    switch (keepRect)
    {
    case ECoordinateSpace::Relative:
    {
        parent_ = parent;
        eventRectUpdated_.Broadcast();
        break;
    }
    case ECoordinateSpace::World:
    {
        const auto size = GetWorldSize();
        const auto position = GetWorldPosition();
        const auto rotation = GetWorldRotation();
        parent_ = parent;
        SetWorldSize(size);
        SetWorldPosition(position);
        SetWorldRotation(rotation);
        break;
    }
    }

    if (parent_)
    {
        parent_->EventRectUpdated().AddListener(KtDelegate(&eventRectUpdated_, &KtEvent<>::Broadcast));
        parent_->AddChildren(Ptr<KInterfaceComponent>());
    }
}

void KInterfaceComponent::SetRelativeSize(const glm::vec2& relativeSize)
{
    if (rect_.size == relativeSize)
    {
        return;
    }

    rect_.size = relativeSize;
    eventRectUpdated_.Broadcast();
}

void KInterfaceComponent::SetRelativePosition(const glm::vec2& relativePosition)
{
    if (rect_.position == relativePosition)
    {
        return;
    }

    rect_.position = relativePosition;
    eventRectUpdated_.Broadcast();
}

void KInterfaceComponent::SetRelativeScale(const glm::vec2& relativeScale)
{
    if (rect_.scale == relativeScale)
    {
        return;
    }

    rect_.scale = relativeScale;
    eventRectUpdated_.Broadcast();
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
    eventRectUpdated_.Broadcast();
}

void KInterfaceComponent::SetWorldSize(const glm::vec2& worldSize)
{
    if (parent_)
    {
        SetRelativeSize(worldSize / parent_->GetWorldScale());
        return;
    }
    SetRelativeSize(worldSize);
}

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

void KInterfaceComponent::SetScreenPosition(const glm::vec2& screenPosition)
{
    const auto viewportSize = glm::vec2(Owner()->GetViewport()->GetExtent());
    const auto newPosition = screenPosition / viewportSize * 2.0f - glm::vec2(1.0f);
    SetWorldPosition(newPosition);
}

void KInterfaceComponent::SetScreenSize(const glm::vec2& screenSize)
{
    const auto viewportSize = glm::vec2(Owner()->GetViewport()->GetExtent());
    const auto newSize = screenSize / viewportSize * 2.0f;
    SetWorldSize(newSize);
}

void KInterfaceComponent::SetAnchor(const EAnchor anchor)
{
    const auto worldPosition = GetWorldPosition();
    rect_.anchor = anchor;
    SetWorldPosition(worldPosition);

}

void KInterfaceComponent::SetColor(const KtColor& color)
{
    color_ = color;
    eventColorUpdated_.Broadcast();
}

float KInterfaceComponent::GetLeft() const
{
    return GetWorldPosition().x - GetWorldSize().x / 2.0f;
}

float KInterfaceComponent::GetRight() const
{
    return GetWorldPosition().x + GetWorldSize().x / 2.0f;
}

float KInterfaceComponent::GetTop() const
{
    return GetWorldPosition().y - GetWorldSize().y / 2.0f;
}

float KInterfaceComponent::GetBottom() const
{
    return GetWorldPosition().y + GetWorldSize().y / 2.0f;
}

glm::mat4 KInterfaceComponent::GetTranslationMatrix() const
{
    return glm::translate(glm::identity<glm::mat4>(), glm::vec3(GetWorldPositionWithAnchorOffset(), 0.0f));
}

glm::mat4 KInterfaceComponent::GetRotationMatrix() const
{
    return glm::rotate(glm::identity<glm::mat4>(), GetWorldRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::mat4 KInterfaceComponent::GetScaleMatrix() const
{
    const auto viewportSize = glm::vec2(Owner()->GetViewport()->GetExtent());
    const float aspectRatio = Owner()->GetViewport()->GetAspectRatio();
    const float rotation = GetWorldRotation(ERotationUnit::Radians);

    // x *= 1 at rot 0
    // y *= 1 at rot 0
    // x *= aspectRatio at rot pi/2
    // y *= 1 / aspectRatio at rot pi/2

    const float value = (cos((rotation + glm::half_pi<float>()) * 2.0f) + 1.0f) / 2.0f;
    const auto stretchCorrection = glm::vec2(std::lerp(1.0f, aspectRatio, value), std::lerp(1.0f, 1.0f / aspectRatio, value));

    const auto size = GetScreenSize() / viewportSize * 2.0f * stretchCorrection;

    return glm::scale(glm::identity<glm::mat4>(), glm::vec3(size, 1.0f));
}

glm::mat4 KInterfaceComponent::GetModelMatrix()
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

bool KInterfaceComponent::GetIsOverlapping(const glm::vec2& worldPosition, const glm::vec2& worldSize) const
{
    const auto distance = glm::abs(GetWorldPosition() - worldPosition);
    const auto maxDistance = (GetWorldSize() + worldSize) / 2.0f;
    return distance.x < maxDistance.x
        && distance.y < maxDistance.y;
}

bool KInterfaceComponent::GetIsOverlapping(const glm::vec2& worldPosition) const
{
    return GetIsOverlapping(worldPosition, glm::vec2(0.0f));
}

bool KInterfaceComponent::GetIsOverlapping(const UPtr<KInterfaceComponent>& other) const
{
    return GetIsOverlapping(other->GetWorldPosition(), other->GetWorldSize());
}

bool KInterfaceComponent::IsHovered() const
{
    const auto& cursorPosition = Framework.InputManager().Mouse().GetCursorPosition();
    const auto viewportSize = glm::vec2(WindowViewport.GetExtent());
    const auto worldPosition = cursorPosition / viewportSize * 2.0f - glm::vec2(1.0f);
    return GetIsOverlapping(worldPosition);
}

void KInterfaceComponent::AddChildren(const UPtr<KInterfaceComponent>& interfaceComponent)
{
    if (!interfaceComponent)
    {
        return;
    }

    children_.Add(interfaceComponent);
    interfaceComponent->childrenIndex_ = static_cast<size_t>(children_.LastIndex());
}

void KInterfaceComponent::RemoveChildren(const UPtr<KInterfaceComponent>& interfaceComponent)
{
    if (!interfaceComponent)
    {
        return;
    }

    const size_t index{ interfaceComponent->childrenIndex_ };
    if (children_.RemoveAt(index) == KtPoolRemoveResult::ItemSwappedAndRemoved)
    {
        children_[index]->childrenIndex_ = index;
    }
}

void KInterfaceComponent::CreateBoundsProxy()
{
    const auto shaderPath = Framework.Path().FrameworkPath() / R"(shaders\flatColor2D.ktshader)";
    const auto texturePath = Framework.Path().FrameworkPath() / R"(assets\textures\white_texture.jpg)";

    boundsProxy_->shader = Framework.ShaderManager().Get(shaderPath);
    boundsProxy_->renderable = Framework.ImageTextureManager().Get(texturePath);
    boundsProxy_->layer = GetLayer();
    boundsProxy_->objectData.modelMatrix = GetModelMatrix();
    boundsProxy_->scissor.offset = WindowViewport.GetOffset();
    boundsProxy_->scissor.extent = WindowViewport.GetExtent();
    boundsProxy_->objectData.color = { 1.0f, 1.0f, 1.0f, 0.01f };
}

void KInterfaceComponent::MarkBoundsProxyRectDirty()
{
    boundsProxy_->isDirty = true;
    boundsProxy_->objectData.modelMatrix = GetModelMatrix();
}

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
