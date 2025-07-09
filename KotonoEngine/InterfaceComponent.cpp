#include "InterfaceComponent.h"
#include "InterfaceObject.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/Renderer.h>
#include <kotono_framework/Path.h>
#include <kotono_framework/Shader.h>
#include <kotono_framework/ShaderManager.h>
#include <kotono_framework/ImageTexture.h>
#include <kotono_framework/ImageTextureManager.h>
#include <kotono_framework/Viewport.h>
#include "log.h"

static KtShader* FlatColorShader = nullptr;
static KtImageTexture* FlatColorTexture = nullptr;

KInterfaceComponent::KInterfaceComponent(RInterfaceObject* owner) : owner_(owner)
{
}

void KInterfaceComponent::Construct()
{
    Base::Construct();

    if (GetOwner()->GetRootComponent() != this)
    {
        SetParent(GetOwner()->GetRootComponent(), ECoordinateSpace::Relative);
    }

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
}

void KInterfaceComponent::Init()
{
    Base::Init();

    InitBoundsProxy();

    visibility_ = EVisibility::EditorAndGame;
}

void KInterfaceComponent::Cleanup()
{
    Base::Cleanup();
    Framework.GetRenderer().GetRenderer2D().Remove(&boundsProxy_);
}

RInterfaceObject* KInterfaceComponent::GetOwner() const
{
    return owner_;
}

KInterfaceComponent* KInterfaceComponent::GetParent() const
{
    return parent_;
}

const URect& KInterfaceComponent::GetRect() const
{
    return rect_;
}

const EVisibility KInterfaceComponent::GetVisibility() const
{
    return visibility_;
}

const int32_t KInterfaceComponent::GetLayer() const
{
    if (parent_)
    {
        return parent_->GetLayer() + layer_ + 1;
    }
    return layer_;
}

KtEvent<>& KInterfaceComponent::GetEventRectUpdated()
{
    return eventRectUpdated_;
}

KtEvent<>& KInterfaceComponent::GetEventLayerUpdated()
{
    return eventLayerUpdated_;
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

const float KInterfaceComponent::GetRelativeRotation(const ERotationUnit unit) const
{
    return rect_.rotation;
}

const glm::vec2 KInterfaceComponent::GetWorldSize() const
{
    return rect_.size * GetWorldScale();
}

const glm::vec2 KInterfaceComponent::GetWorldPosition() const
{
    if (parent_)
    {
        return rect_.position + parent_->GetWorldPosition();
    }
    return rect_.position;
}

const glm::vec2 KInterfaceComponent::GetWorldScale() const
{
    if (parent_)
    {
        return rect_.scale * parent_->GetWorldScale();
    }
    return rect_.scale;
}

const float KInterfaceComponent::GetWorldRotation(const ERotationUnit unit) const
{
    if (parent_)
    {
        return GetRelativeRotation(unit) + parent_->GetWorldRotation(unit);
    }
    return GetRelativeRotation(unit);
}

const glm::vec2 KInterfaceComponent::GetScreenPosition() const
{
    const auto viewportSize = glm::vec2(owner_->GetViewport()->GetExtent());
    const auto newPosition = (GetWorldPosition() + glm::vec2(1.0f)) * viewportSize / 2.0f;
    return newPosition;
}

const glm::vec2 KInterfaceComponent::GetScreenSize() const
{
    const auto viewportSize = glm::vec2(owner_->GetViewport()->GetExtent());
    const auto newSize = GetWorldSize() * viewportSize / 2.0f;
    return newSize;
}

const EAnchor KInterfaceComponent::GetAnchor() const
{
    return rect_.anchor;
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

void KInterfaceComponent::SetParent(KInterfaceComponent* parent, const ECoordinateSpace keepRect)
{
    if (parent_)
    {
        parent_->GetEventRectUpdated().RemoveListener(KtDelegate<>(&eventRectUpdated_, &KtEvent<>::Broadcast));
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
        parent_->GetEventRectUpdated().AddListener(KtDelegate<>(&eventRectUpdated_, &KtEvent<>::Broadcast));
    }
}

void KInterfaceComponent::SetRelativeSize(const glm::vec2& relativeSize)
{
    rect_.size = relativeSize;
    eventRectUpdated_.Broadcast();
}

void KInterfaceComponent::SetRelativePosition(const glm::vec2& relativePosition)
{
    rect_.position = relativePosition;
    eventRectUpdated_.Broadcast();
}

void KInterfaceComponent::SetRelativeScale(const glm::vec2& relativeScale)
{
    rect_.scale = relativeScale;
    eventRectUpdated_.Broadcast();
}

void KInterfaceComponent::SetRelativeRotation(float relativeRotation, const ERotationUnit unit)
{
    if (unit == ERotationUnit::Degrees)
    {
        relativeRotation = glm::radians(relativeRotation);
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
    const auto viewportSize = glm::vec2(GetOwner()->GetViewport()->GetExtent());
    const auto newPosition = screenPosition / viewportSize * 2.0f - glm::vec2(1.0f);
    SetWorldPosition(newPosition);
}

void KInterfaceComponent::SetScreenSize(const glm::vec2& screenSize)
{
    const auto viewportSize = glm::vec2(GetOwner()->GetViewport()->GetExtent());
    const auto newSize = screenSize / viewportSize * 2.0f;
    SetWorldSize(newSize);
}

void KInterfaceComponent::SetAnchor(const EAnchor anchor)
{
    const auto worldPosition = GetWorldPosition();
    rect_.anchor = anchor;
    SetWorldPosition(worldPosition);

}

const float KInterfaceComponent::GetLeft() const
{
    return GetWorldPosition().x - GetWorldSize().x / 2.0f;
}

const float KInterfaceComponent::GetRight() const
{
    return GetWorldPosition().x + GetWorldSize().x / 2.0f;
}

const float KInterfaceComponent::GetTop() const
{
    return GetWorldPosition().y - GetWorldSize().y / 2.0f;
}

const float KInterfaceComponent::GetBottom() const
{
    return GetWorldPosition().y + GetWorldSize().y / 2.0f;
}

const glm::mat4 KInterfaceComponent::GetTranslationMatrix() const
{
    return glm::translate(glm::identity<glm::mat4>(), glm::vec3(GetWorldPositionWithAnchorOffset(), 0.0f));
}

const glm::mat4 KInterfaceComponent::GetRotationMatrix() const
{
    return glm::rotate(glm::identity<glm::mat4>(), GetWorldRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
}

const glm::mat4 KInterfaceComponent::GetScaleMatrix() const
{
    const auto viewportSize = glm::vec2(GetOwner()->GetViewport()->GetExtent());
    const float aspectRatio = GetOwner()->GetViewport()->GetAspectRatio();
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

const glm::mat4 KInterfaceComponent::GetModelMatrix() const
{
    return GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
}

const glm::vec2 KInterfaceComponent::GetDirection(const KInterfaceComponent* target) const
{
    return target->GetWorldPosition() - GetWorldPosition();
}

const float KInterfaceComponent::GetDistance(const KInterfaceComponent* other) const
{
    return glm::distance(GetWorldPosition(), other->GetWorldPosition());
}

const bool KInterfaceComponent::GetIsOverlapping(const glm::vec2& worldPosition, const glm::vec2& worldSize) const
{
    const auto distance = glm::abs(GetWorldPosition() - worldPosition);
    const auto maxDistance = (GetWorldSize() + worldSize) / 2.0f;
    return distance.x < maxDistance.x
        && distance.y < maxDistance.y;
}

const bool KInterfaceComponent::GetIsOverlapping(const glm::vec2& worldPosition) const
{
    return GetIsOverlapping(worldPosition, glm::vec2(0.0f));
}

const bool KInterfaceComponent::GetIsOverlapping(const KInterfaceComponent* other) const
{
    return GetIsOverlapping(other->GetWorldPosition(), other->GetWorldSize());
}

void KInterfaceComponent::InitBoundsProxy()
{
    CreateBoundsProxy();
    Framework.GetRenderer().GetRenderer2D().Register(&boundsProxy_);

    const KtDelegate<> rectDelegate(this, &KInterfaceComponent::MarkBoundsProxyRectDirty);
    ListenEvent(GetEventRectUpdated(), rectDelegate);
}

void KInterfaceComponent::CreateBoundsProxy()
{
    boundsProxy_.shader = FlatColorShader;
    boundsProxy_.viewport = GetOwner()->GetViewport();
    boundsProxy_.renderable = FlatColorTexture;
    boundsProxy_.layer = GetLayer();
    boundsProxy_.objectData.modelMatrix = GetModelMatrix();
}

void KInterfaceComponent::MarkBoundsProxyRectDirty()
{
    boundsProxy_.isDirty = true;
    boundsProxy_.objectData.modelMatrix = GetModelMatrix();
    KT_LOG_KE(KT_LOG_COMPILE_TIME_LEVEL, "bounds rect dirty");
}

const glm::vec2 KInterfaceComponent::GetAnchorOffset() const
{
    return glm::vec2(
        (GetAnchor() & EAnchor::Left) == EAnchor::Left ? GetRelativeScale().x / 2.0f
        : (GetAnchor() & EAnchor::Right) == EAnchor::Right ? -GetRelativeScale().x / 2.0f
        : 0.0f,
        (GetAnchor() & EAnchor::Top) == EAnchor::Top ? GetRelativeScale().y / 2.0f
        : (GetAnchor() & EAnchor::Bottom) == EAnchor::Bottom ? -GetRelativeScale().y / 2.0f
        : 0.0f
    );
}

const glm::vec2 KInterfaceComponent::GetWorldPositionWithAnchorOffset() const
{
    if (parent_)
    {
        return GetRelativePosition() + GetAnchorOffset() + parent_->GetWorldPositionWithAnchorOffset();
    }
    return GetRelativePosition() + GetAnchorOffset();
}

const glm::vec2 KInterfaceComponent::GetAnchorRelativePosition() const
{
    return GetRelativePosition() - GetAnchorOffset();
}
